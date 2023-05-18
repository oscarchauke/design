const socket = new WebSocket('ws://192.168.4.1:81');

document.addEventListener("DOMContentLoaded", function () {
    document.getElementById("record-data").addEventListener("click", function () {
        document.getElementById("select-group").style.display = "block";
        document.getElementById("welcome").style.display = "none";
    });

    document.getElementById("view-data").addEventListener("click", function () {
        document.getElementById("view-data-page").style.display = "block";
        document.getElementById("welcome").style.display = "none";
    });

    document.getElementById("group-1").addEventListener("click", function () {
        document.getElementById("select-group").style.display = "none";
        document.getElementById("live-data-page").style.display = "block";
        if (socket.readyState === WebSocket.OPEN) {
          const message = 'group:1'; // Replace with your desired message
          socket.send(message);
           } else {
          console.log('WebSocket connection is not open.');
        }
    });

    document.getElementById("go-back").addEventListener("click", function () {
        document.getElementById("welcome").style.display = "block";
        document.getElementById("live-data-page").style.display = "none";
    });
});

const options = {
  chart: {
    type: 'scatter',
    height: 400,
    animations: {
      enabled: false,
    },
  },
  series: [{
    name: 'Scatter Data',
    data: [], // Initial empty data
  }],
  xaxis: {
    type: 'numeric',
  },
  yaxis: {
    type: 'numeric',
  },
};

const chart = new ApexCharts(document.querySelector("#chart"), options);
chart.render();


// Step 3: Update chart data on WebSocket message
socket.addEventListener('message', (event) => {
  const newData = JSON.parse(event.data);
  console.log('Received data:', newData);

  // Assuming newData is an array of objects with 'x' and 'y' properties
  const updatedSeries = [{
    name: 'Scatter Data',
    data: newData,
  }];

  chart.updateSeries(updatedSeries);
});

//send time to server
socket.addEventListener('open', () => {
  console.log("socket opened");
  document.getElementById("welcome").style.display = "block";
  const currentTime = new Date().getTime()/1000;
  
  const timeData = {
    time: currentTime.toString()
  };
  
  const timeDataJson = JSON.stringify(timeData);
  
  socket.send(timeDataJson);
});