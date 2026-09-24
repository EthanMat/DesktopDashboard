import ICAL from "../vendor/ical.js";

async function fetchIcsData(url) {
  const response = await fetch(url);
  if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
  return response.text();
}

async function readFile(filePath, lineIndex) {
  const response = await fetch(filePath);
  if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
  return (await response.text()).split('\n')[lineIndex].trim();
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

async function displayEvents(events){
  var events = parseIcs(await fetchIcsData(await readFile("./config.txt", 0)));
  var list = document.getElementById('calendar');
  list.innerHTML = ""; // Clear previous events
  console.log(events);
  for (var event of events) {
    if (event.startDate < new Date()) continue; // Skip past events
    if (event.startDate > new Date(Date.now() + 1 * 24 * 60 * 60 * 1000)) break; // Skip events more than a day away
    var eventElement = document.createElement('div');
    eventElement.className = 'calendar-event';
    var timeUntilEvent = Math.round((event.startDate - new Date()) / (1000 * 60)); // Time until event in minutes
    if (timeUntilEvent <= 60) {
      eventElement.innerHTML = `<h4>${event.title} - ${event.location}</h4><p>${event.startDate.toLocaleTimeString([], {hour: 'numeric', minute: '2-digit'})} - in ${timeUntilEvent} minutes</p>`;
      eventElement.style.color = 'red';
    } else {
      eventElement.innerHTML = `<h4>${event.title} - ${event.location}</h4><p>${event.startDate.toLocaleTimeString([], {hour: 'numeric', minute: '2-digit'})} - in ${Math.round(timeUntilEvent / 60)} hours</p>`;
    }
    list.appendChild(eventElement);
  }

  if (list.children.length === 0) {
    var noEventsElement = document.createElement('div');
    noEventsElement.className = 'no-events';
    noEventsElement.textContent = 'No upcoming events within the next 24 hours.';
    list.appendChild(noEventsElement);
  }
}

displayEvents();
setInterval(displayEvents, 1 * 60 * 1000); // Refresh every minute