
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

/**
 * The basic math module.
 */
FABRIC.Math = FABRIC.Math ? FABRIC.Math : {};

/**
 * Function to determine if an object is a valid color.
 * @param {object} color The color object to validate.
 * @return {boolean} true if the given object is a valid color.
 */
FABRIC.Math.isColor = function(color) {
  return typeof color === 'object' &&
    'r' in color &&
    typeof color.r === 'number' &&
    'g' in color &&
    typeof color.g === 'number' &&
    'b' in color &&
    typeof color.b === 'number' &&
    'a' in color &&
    typeof color.a === 'number';
};

/**
 * Constructor for a color object.
 * @constructor
 * @param {number} r The red component.
 * @param {number} g The green component.
 * @param {number} b The blue  component.
 * @param {number} a The alpha component.
 */
FABRIC.Math.Color = function(r, g, b, a) {
  if (typeof r === 'number' && typeof g === 'number' && typeof b === 'number' && typeof a === 'number') {
    this.r = r;
    this.g = g;
    this.b = b;
    this.a = a;
  }
  else if (typeof r === 'number' && typeof g === 'number' && typeof b === 'number' && a === undefined) {
    this.r = r;
    this.g = g;
    this.b = b;
    this.a = 1.0;
  }
  else if (FABRIC.Math.isColor(r) && g === undefined && b === undefined && a === undefined) {
    this.r = r.r;
    this.g = r.g;
    this.b = r.b;
    this.a = r.a;
  }
  else if (r === undefined && g === undefined && b === undefined && a === undefined) {
    this.r = 0;
    this.g = 0;
    this.b = 0;
    this.a = 1;
  }
  else throw'new Color: invalid arguments';
  };

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @param {number} r The red component.
 * @param {number} g The green component.
 * @param {number} b The blue  component.
 * @param {number} a The alpha component.
 * @return {object} The color object.
 */
FABRIC.Math.color = function(r, g, b, a) {
  return new FABRIC.Math.Color(r, g, b, a);
};

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @param {number} r The red component.
 * @param {number} g The green component.
 * @param {number} b The blue  component.
 * @return {object} The color object.
 */
FABRIC.Math.rgb = function(r, g, b) {
  return new FABRIC.Math.Color(r, g, b, 1);
};

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @param {number} r The red component.
 * @param {number} g The green component.
 * @param {number} b The blue  component.
 * @param {number} a The alpha component.
 * @return {object} The color object.
 */
FABRIC.Math.rgba = function(r, g, b, a) {
  return new FABRIC.Math.Color(r, g, b, a);
};

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @param {number} r The red component.
 * @param {number} g The green component.
 * @param {number} b The blue  component.
 * @return {object} The color object.
 */
FABRIC.Math.rgb255 = function(r, g, b) {
  return new FABRIC.Math.Color(r / 255, g / 255, b / 255, 1);
};

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @param {number} r The red component.
 * @param {number} g The green component.
 * @param {number} b The blue  component.
 * @param {number} a The alpha component.
 * @return {object} The color object.
 */
FABRIC.Math.rgba255 = function(r, g, b, a) {
  return new FABRIC.Math.Color(r / 255, g / 255, b / 255, a / 255);
};

FABRIC.Math.Color.prototype = {
  setRGB: function(r, g, b) {
    this.r = r;
    this.g = g;
    this.b = b;
  },
  setRGBA: function(r, g, b, a) {
    this.r = r;
    this.g = g;
    this.b = b;
    this.a = a;
  },
  setRGB255: function(r, g, b) {
    this.r = r / 255;
    this.g = g / 255;
    this.b = b / 255;
  },
  setRGBA255: function(r, g, b, a) {
    this.r = r / 255;
    this.g = g / 255;
    this.b = b / 255;
    this.a = a / 255;
  },
  clampColorValues: function() {
    this.r = Math.min(Math.max(this.r, 0), 1);
    this.g = Math.min(Math.max(this.g, 0), 1);
    this.b = Math.min(Math.max(this.b, 0), 1);
    this.a = Math.min(Math.max(this.a, 0), 1);
    return this;
  },

  // Returns true if the vector is equal to the argument
  eql: function(color) {
    return (FABRIC.Math.isColor(color) &&
      (Math.abs(this.r - color.r) < FABRIC.Math.precision) &&
      (Math.abs(this.g - color.g) < FABRIC.Math.precision) &&
      (Math.abs(this.b - color.b) < FABRIC.Math.precision) &&
      (Math.abs(this.a - color.a) < FABRIC.Math.precision));
  },

  // Returns the result of adding the argument to the vector
  add: function(col) {
    var c = new FABRIC.Math.Color(this.r + col.r, this.g + col.g, this.b + col.b, this.a + col.a);
    c.clampColorValues();
    return c;
  },

  addInPlace: function(col) {
    this.r += col.r; this.g += col.g; this.b += col.b; this.a += col.a;
    return this.clampColorValues();
  },

  subtract: function(col) {
    var c = (new FABRIC.Math.Color(this.r - col.r, this.g - col.g, this.b - col.b, this.a - col.a)).clampColorValues();
    c.clampColorValues();
    return c;
  },

  subInPlace: function(col) {
    this.r -= col.r; this.g -= col.g; this.b -= col.b; this.a -= col.a;
    return this.clampColorValues();
  },

  multiply: function(col) {
    if (typeof col == 'number') {
      var c = new FABRIC.Math.Color(this.r * col, this.g * col, this.b * col, this.a * col);
      return c.clampColorValues();
    }else if (isColor(col)) {
      var c = new FABRIC.Math.Color(this.r * col.r, this.g * col.g, this.b * col.b, this.a * col.a);
      return c.clampColorValues();
    }else {
      log('Incorrect param type for Multiply');
    }
  },

  mulInPlace: function(col) {
    if (typeof col == 'number') {
      this.r *= col; this.g *= col; this.b *= col; this.a *= col;
    }else {
      this.r *= col.r; this.g *= col.g; this.b *= col.b; this.a *= col.a;
    }
    this.clampColorValues();
    return this;
  },

  // Returns the scalar product of the vector with the argument
  // Both vectors must have equal dimensionality
  dot: function(col) {
    if (isColor(col)) {
      return (this.r * col.r) + (this.g * col.g) + (this.b * col.b) + (this.a * col.a);
    }
  },

  // Returns the length ('length') of the vector
  length: function() {
    return Math.sqrt(this.dot(this));
  },
  norm: function() {
    return this.length();
  },

  // Normalized this vector and returns the previous length
  normalize: function() {
    var len = this.length();
    if (len === 0) {
      return 0;
    }
    this.mulInPlace(1.0 / len);
    return len;
  },

  interpTowards: function(col, weight) {
    var delta = col.subtract(this);
    delta.mulInPlace(weight);
    return this.add(delta);
  },

  toHex: function() {
    var toHex = function(N) {
      if (N == null)return '00';
        N = parseInt(N); if (N == 0 || isNaN(N))return '00';
      N = Math.max(0, N); N = Math.min(N, 255); N = Math.round(N);
      return '0123456789ABCDEF'.charAt((N - N % 16) / 16) +
        '0123456789ABCDEF'.charAt(N % 16);
    };
    return toHex(255 * this.r) + toHex(255 * this.g) + toHex(255 * this.b); // + toHex(255*this.a);
  },

  toRGBStr: function() {
    return 'FABRIC.Math.rgb(' + this.r + ',' + this.g + ',' + this.b + ')';
  },
  // Used to convert FABRIC color values to DOM color values.
  toRGB255Str: function() {
    return 'rgb(' + Math.round(this.r * 255) + ',' + Math.round(this.g * 255) + ',' + Math.round(this.b * 255) + ')';
  },
  clone: function() {
    return new FABRIC.Math.Color(this);
  },
  toString: function() {
    return 'FABRIC.Math.rgba(' + this.r + ',' + this.g + ',' + this.b + ',' + this.a + ')';
  },
  getType: function() {
    return 'FABRIC.Math.Color';
  }/*,
  displayGUI:function($parentDiv, changeHandlerFn, size){
    var val = this;
    try{
      var $widget = $("<p></p>").ColorPicker(
        {
          flat: true,
          color: { r:this.r*255, g:this.g*255, b:this.b*255} ,
          onChange: function (hsb, hex, rgb) {
            val.setRGB255(rgb.r, rgb.g, rgb.b);
            changeHandlerFn(val);
          }
        });
      $parentDiv.append($widget);
    }catch(e){
      console.error("Missing jQuery Color Picker Plugin");
    }
  }*/
};

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @return {object} The color object.
 */
FABRIC.Math.Color.white = FABRIC.Math.rgb255(255, 255, 255);

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @return {object} The color object.
 */
FABRIC.Math.Color.red = FABRIC.Math.rgb255(255, 0, 0);

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @return {object} The color object.
 */
FABRIC.Math.Color.green = FABRIC.Math.rgb255(0, 255, 0);

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @return {object} The color object.
 */
FABRIC.Math.Color.blue = FABRIC.Math.rgb255(0, 0, 255);

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @return {object} The color object.
 */
FABRIC.Math.Color.cyan = FABRIC.Math.rgb255(0, 255, 255);

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @return {object} The color object.
 */
FABRIC.Math.Color.magenta = FABRIC.Math.rgb255(255, 0, 255);

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @return {object} The color object.
 */
FABRIC.Math.Color.yellow = FABRIC.Math.rgb255(255, 255, 0);

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @return {object} The color object.
 */
FABRIC.Math.Color.brown = FABRIC.Math.rgb255(50, 100, 155);

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @return {object} The color object.
 */
FABRIC.Math.Color.lightgrey = FABRIC.Math.rgb255(155, 155, 155);

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @return {object} The color object.
 */
FABRIC.Math.Color.grey = FABRIC.Math.rgb255(100, 100, 100);

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @return {object} The color object.
 */
FABRIC.Math.Color.darkgrey = FABRIC.Math.rgb255(50, 50, 50);

/**
 * Overloaded constructor function for FABRIC.Math.Color
 * @return {object} The color object.
 */
FABRIC.Math.Color.black = FABRIC.Math.rgb255(0, 0, 0);

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('Color', {
    members: {
      r: 'Scalar', g: 'Scalar', b: 'Scalar', a: 'Scalar'
    },
    constructor: FABRIC.Math.Color,
    kBindings: FABRIC.loadResourceURL('../../../SceneGraph/Resources//RT/Color.kl')
  });
});
