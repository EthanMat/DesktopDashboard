var currentTime = new Date();

function updateClock() {
    currentTime = new Date();
    var hours = currentTime.getHours();
    var minutes = currentTime.getMinutes();
    var seconds = currentTime.getSeconds();

    var ampm = hours >= 12 ? 'PM' : 'AM';

    if (hours > 12) {
        hours -= 12;
    }

    if (hours == 0) {
        hours = 12;
    }

    // Format the time as HH:MM:SS
    var formattedTime = hours.toString().padStart(2, '0') + ':' +
                        minutes.toString().padStart(2, '0') + ':' +
                        seconds.toString().padStart(2, '0') + ' ' + ampm;
    
    // Update the clock element with the formatted time
    document.getElementById('clock').textContent = formattedTime;
}

// Update the clock every second
setInterval(updateClock, 1000);