module.exports = {
  decompose: function (A) {
    var n = A.length;
    console.log(A);
    
    var L = [];
    for (var i=0; i<n; ++i) {
      L[i] = [];
      for (var j=0; j<n; ++j)
        L[i].push(0.0);
    }

    for (var i=0; i<n; ++i) {
      for (var j=0; j<(i+1); ++j ) {
        var s = 0.0;
        for (var k=0; k<j; ++k)
          s += L[i][k] * L[j][k];
        if (i == j)
          L[i][j] = Math.sqrt(A[i][i]-s);
        else
          L[i][j] = 1.0/L[j][j]*(A[i][j]-s);
      }
    }
    return L;
  }
};
