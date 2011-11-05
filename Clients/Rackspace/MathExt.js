module.exports = {
  randomNormal: function(mean, dev) {
    if (mean === undefined)
      mean = 0.0;
    if (dev === undefined)
      dev = 1.0;

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
    return result * dev + mean;
  },

  randomNormalVec: function (n, mean, dev) {
    if (n === undefined)
      throw "vec.randomNormal: missing length";
    var v = [];
    for (var i=0; i<n; ++i)
      v[i] = this.randomNormal(mean, dev);
    return v;
  },

  mat: {
    zero: function (rows, cols) {
      var M = [];
      for (var i=0; i<rows; ++i) {
        M[i] = [];
        for (var j=0; j<cols; ++j)
          M[i][j] = 0.0;
      }
      return M;
    },

    mulVec: function (M, v) {
      var r = [];
      for (var i=0; i<M.length; ++i) {
        var sum = 0.0;
        for (var j=0; j<v.length; ++j)
          sum += M[i][j] * v[j];
        r[i] = sum;
      }
      return v;
    },

    mul: function (M, N) {
      var R = [];
      for (var i=0; i<M.length; ++i) {
        if (M[i].length != N.length)
          throw "bad matricies";
        R[i] = [];
        for (var j=0; j<N[0].length; ++j) {
          var sum = 0;
          for (var k=0; k<N.length; ++k)
            sum += M[i][k] * N[k][j];
          R[i][j] = sum;
        }
      }
      return R;
    },

    trans: function (M) {
      var R = [];
      for (var i=0; i<M[0].length; ++i) {
        R[i] = [];
        for (var j=0; j<M.length; ++j) {
          R[i][j] = M[j][i];
        }
      }
      return R;
    }
  },

  randomCorrelation: function (n) {
    var T = [];

    // Generate n uniform random columns
    for (var i=0; i<n; ++i) {
      T[i] = [];
      for (var j=0; j<n; ++j) {
        T[i][j] = this.randomNormal();
      }
    }

    // Normalize the columns
    for (var j=0; j<n; ++j) {
      var sqSum = 0;
      for (var i=0; i<n; ++i)
        sqSum += T[i][j] * T[i][j];
      var norm = Math.sqrt(sqSum);
      for (var i=0; i<n; ++i)
        T[i][j] /= norm;
    }

    // result is T'*T
    return this.mat.mul(this.mat.trans(T), T);
  },

  choleskyTrans: function (A) {
    var n = A.length;
    
    var L = this.mat.zero(n, n);
    for (var i=0; i<n; ++i) {
      for (var j=0; j<(i+1); ++j ) {
        var s = 0.0;
        for (var k=0; k<j; ++k)
          s += L[i][k] * L[j][k];
        if (i == j)
          L[i][i] = Math.sqrt(A[i][i]-s);
        else
          L[i][j] = 1.0/L[j][j]*(A[i][j]-s);
      }
    }
    return L;
  },

  cholesky: function (A) {
    return this.mat.trans(this.choleskyTrans(A));
  }
};
