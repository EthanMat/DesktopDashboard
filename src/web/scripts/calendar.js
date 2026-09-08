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
events.forEach(e => {
  const row = document.createElement('div');
  row.className = 'event-row';
  row.innerHTML = `<span class="event-time">${e.startDate.toLocaleTimeString([], {hour:'numeric', minute:'2-digit'})}</span><span class="event-title">${e.title}</span>`;
  list.appendChild(row);
});
console.log(events);