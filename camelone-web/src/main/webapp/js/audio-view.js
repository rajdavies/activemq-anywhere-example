function AudioView() {

    this.MAX_DATA_SIZE = 100;
    this.height = 500;
    this.elementWidth = 1024;
    this.data = [];
    this.initialize();
}

AudioView.prototype.initialize = function () {
    for (var i = 0; i < this.MAX_DATA_SIZE; i++) {
        this.data.push(0);
    }
    this._y = d3.scale.linear()
            .domain([0, this.height])
            .rangeRound([0, this.height]);
    this.color = d3.scale.linear()
            .domain([0, 500])
            .range(["blue", "red"]);
    this.amplitude = d3.scale.linear()
            .domain([0, 500])
            .range([0, 450]);
    this.createChart();
    this.initializeChart();
}


AudioView.prototype.createChart = function () {
    this.chart = d3.select("#audio").append("svg")
            .attr("class", "chart")
            .attr("width", this.barWidth() * this.MAX_DATA_SIZE)
            .attr("height", this.height);
}


AudioView.prototype.initializeChart = function () {
    var view = this;
    this.chart.selectAll("rect")
            .data(this.data)
            .enter().append("rect")
            .attr("fill", "#FFF")
            .attr("x", function (d, i) {
                return view._x(i) - .5;
            })
            .attr("y", function (d) {
                return view.height - view._y(d) - .5;
            })
            .attr("width", this.barWidth())
            .attr("height", function (d) {
                return 0;
            });
}


AudioView.prototype._x = function (n) {
    return d3.scale.linear()
            .domain([0, 1])
            .range([0, this.barWidth()])(n);
}

AudioView.prototype.barWidth = function () {
    var dataLength = Math.max(this.data.length, 1);
    return this.elementWidth / this.MAX_DATA_SIZE;
}

AudioView.prototype.onAudio = function (value) {

    this.data.push(value);
    console.log("amplitude = " + value);

    // pop the old data point off the front
    if (this.data.length >= this.MAX_DATA_SIZE) {
        this.data.shift();
    }
    this.update();

}

AudioView.prototype.update = function () {
    var view = this;
    this.chart.selectAll("rect")
            .data(this.data)
            .attr("fill", function (d) {
                return view.color(d);
            })
            .attr("x", function (d, i) {
                return view._x(i);
            })
            .attr("y", function (d) {
                return view.height - view._y(d);
            })
            .attr("width", this.barWidth())
            .attr("height", function (d) {
                return view.amplitude(d);

            });
}


AudioView.prototype.close = function () {
    var svg = d3.select("svg");
    if (svg != null) {
        svg.remove();
    }
}


