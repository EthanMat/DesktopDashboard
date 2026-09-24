async function readFile(filePath, lineIndex) {
  const response = await fetch(filePath);
  if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
  return (await response.text()).split('\n')[lineIndex].trim();
}

async function fetchWeather() {
    const apiKey = await readFile("./config.txt", 1); // Read the API key from the second line of config.txt
    const city = await readFile("./config.txt", 2); // Read the city from the third line of config.txt
    const url = `https://api.openweathermap.org/data/2.5/weather?q=${city}&appid=${apiKey}`;
    const response = await fetch(url);
    if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
    return response.json();
}



console.log(await fetchWeather());