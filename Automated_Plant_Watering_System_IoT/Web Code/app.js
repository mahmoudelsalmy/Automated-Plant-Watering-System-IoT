let soil = 0;
let tank = 0;
let pump = "AUTO";

// ============================
// Gauges
// ============================
var soilGauge = new Gauge(document.getElementById("soilGauge")).setOptions({
    colorStart: "#2ecc71",
    colorStop: "#e74c3c",
    strokeColor: "#222",
    pointer: { length: 0.6, color:'#fff' },
    angle: 0,
    lineWidth: 0.22,
});
soilGauge.maxValue = 100;
soilGauge.setMinValue(0);

var tankGauge = new Gauge(document.getElementById("tankGauge")).setOptions({
    colorStart: "#3498db",
    colorStop: "#9b59b6",
    strokeColor: "#222",
    pointer: { length: 0.6, color:'#fff' },
    angle: 0,
    lineWidth: 0.22,
});
tankGauge.maxValue = 100;
tankGauge.setMinValue(0);

// ============================
// Graph
// ============================
const ctx = document.getElementById('graph').getContext('2d');
let chart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: 'Soil %',
            borderColor: '#2ecc71',
            data: [],
        },{
            label: 'Tank %',
            borderColor: '#3498db',
            data: [],
        }]
    },
    options: {
        animation: false,
        scales: {
            y: { min:0, max:100 }
        }
    }
});

// ============================
// Pump control
// ============================
function setPump(mode) {
    pump = mode;
    document.getElementById("pumpState").innerText = pump;
}

// ============================
// UI update
// ============================
function updateUI() {
    soilGauge.set(soil);
    tankGauge.set(tank);

    chart.data.labels.push(new Date().toLocaleTimeString());
    chart.data.datasets[0].data.push(soil);
    chart.data.datasets[1].data.push(tank);
    chart.update();
}

// ============================
// Simulated incoming data
// ============================
function simulateIncomingData() {
    soil = Math.floor(Math.random() * 100);
    tank = Math.floor(Math.random() * 100);
    updateUI();
}

// update every 2s
setInterval(simulateIncomingData, 2000);
