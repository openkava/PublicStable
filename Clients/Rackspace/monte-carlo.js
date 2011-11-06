MathExt = require('./MathExt.js');

var numStocks = 4;
var numTradingDays = 252;
var dt = 1.0/numTradingDays;
var sqrtDT = Math.sqrt(dt);

//var priceMeans = MathExt.randomNormalVec(numStocks,5.0/numTradingDays,1.0/numTradingDays);
var priceMeans = [];
for (var i=0; i<numStocks; ++i)
  priceMeans[i] = 10.0/numTradingDays;

//var priceDevs = MathExt.randomNormalVec(numStocks,1.0/numTradingDays,0.1/numTradingDays);
var priceDevs = [];
for (var i=0; i<numStocks; ++i)
  priceDevs[i] = 2.0/numTradingDays;

var priceCorrelations = MathExt.randomCorrelation(numStocks);
console.log("priceCorrelations:");
console.log(priceCorrelations);

var priceCovariance = [];
for (var i=0; i<numStocks; ++i) {
  priceCovariance[i] = [];
  for (var j=0; j<numStocks; ++j) {
    priceCovariance[i][j] = priceDevs[i] * priceDevs[j] * priceCorrelations[i][j];
  }
}
console.log("priceCovariance:");
console.log(priceCovariance);

var choleskyTrans = MathExt.choleskyTrans(priceCovariance);

var drifts = [];
for (var i=0; i<numStocks; ++i)
  drifts[i] = priceMeans[i] - priceCovariance[i][i]/2;

var numTrials = 10000;
var trialResults = [];
for (var trial=0; trial<numTrials; ++trial) {
  var amounts = [];
  for (var i=0; i<numStocks; ++i)
    amounts[i] = 100;

  for (var day=1; day<=numTradingDays; ++day) {
    var Z = MathExt.randomNormalVec(numStocks);
    var X = MathExt.mat.mulVec(choleskyTrans, Z);
    for (var i=0; i<numStocks; ++i) {
      amounts[i] *= Math.exp(drifts[i]*dt + X[i]*sqrtDT);
    }
  }

  var value = 0.0;
  for (var i=0; i<numStocks; ++i)
    value += amounts[i];
  trialResults.push(value);
}

var sort = function (v) {
  var partition = function (a, begin, end, pivot) {
    var piv = a[pivot];
    a[pivot] = a[end-1];
    a[end-1] = piv;
    var store = begin;
    for (var i=begin; i<end-1; ++i) {
      if (a[i] <= piv) {
        var t = a[store];
        a[store] = a[i];
        a[i] = t;
        ++store;
      }
    }
    var t = a[end-1];
    a[end-1] = a[store];
    a[store] = t;
    return store;
  };

  var qsort = function (a, begin, end) {
    if (end - begin <= 1)
      return;
    else {
      var pivot = partition(a, begin, end, begin+Math.round((end-begin)/2));
      qsort(a, begin, pivot);
      qsort(a, pivot+1, end);
    }
  };

  return qsort(v, 0, v.length);
};

sort(trialResults);
console.log("ValueAtRisk = " + ((numStocks * 100.0) - trialResults[numTrials*0.05]));
