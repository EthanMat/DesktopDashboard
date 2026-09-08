import ICAL from "../vendor/ical.js";

async function fetchIcsData(url) {
  const response = await fetch(url);
  if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
  return response.text();
}

async function readFile(filePath) {
  const response = await fetch(filePath);
  if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
  return response.text();
}

function parseIcs(icsText) {
  const jcalData = ICAL.parse(icsText);
  const comp = new ICAL.Component(jcalData);
  return comp.getAllSubcomponents('vevent').map(vevent => {
    const ev = new ICAL.Event(vevent);
    return {
      title: ev.summary,
      startDate: ev.startDate.toJSDate(),
      endDate: ev.endDate.toJSDate(),
      description: ev.description,
      location: ev.location
    };
  });
}

//Temp check for testing
const events = parseIcs(await fetchIcsData(await readFile("./config.txt")));
const list = document.getElementById('calendar');
// events.forEach(e => {
//   const row = document.createElement('div');
//   row.className = 'event-row';
//   row.innerHTML = `<span class="event-time">${e.startDate.toLocaleTimeString([], {hour:'numeric', minute:'2-digit'})}</span><span class="event-title">${e.title}</span>`;
//   list.appendChild(row);
// });

for (const event of events) {
  if (event.startDate < new Date()) continue; // Skip past events
  if (event.startDate > new Date(Date.now() + 1 * 24 * 60 * 60 * 1000)) break; // Skip events more than a day away
  const eventElement = document.createElement('div');
  eventElement.className = 'calendar-event';
  const timeUntilEvent = Math.max(0, Math.floor((event.startDate - new Date()) / (1000 * 60))); // Time until event in minutes
  if (timeUntilEvent <= 60) {
    eventElement.style.color = 'red'; // Highlight events within the next hour
    eventElement.innerHTML = `<h4>${event.title} - ${event.location}</h4><p>${event.startDate.toLocaleTimeString([], {hour: 'numeric', minute: '2-digit'})} - in ${timeUntilEvent} minutes</p>`;
  } else {
    eventElement.innerHTML = `<h4>${event.title} - ${event.location}</h4><p>${event.startDate.toLocaleTimeString([], {hour: 'numeric', minute: '2-digit'})} - in ${Math.floor(timeUntilEvent / 60)} hours</p>`;
  }
  list.appendChild(eventElement);
}
console.log(events);