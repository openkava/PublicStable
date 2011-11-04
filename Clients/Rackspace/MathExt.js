module.exports = {
  randomNormal: function() {
    var result;
    if ("randomNormalCache" in this) {
      result = this.randomNormalCache;
      delete this.randomNormalCache;
    }
    else {
      var x1, x2, w;
      do {
        x1 = 2.0 * Math.random() - 1.0;
        x2 = 2.0 * Math.random() - 1.0;
        w = x1 * x1 + x2 * x2;
      } while (w >= 1.0);
      w = Math.sqrt( (-2.0 * Math.log(w)) / w );
      result = x1 * w;
      this.randomNormalCache = x2 * w;
    }
    return result;
  },

  cholesky: function (A) {
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
