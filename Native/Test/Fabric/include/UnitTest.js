FABRIC.UnitTest = function() {
  var indentLevel = 0;

  var logCallback = function( level, indentLevel, description ) {
    var line = "";
    for ( var i=0; i<indentLevel; ++i )
      line += "  ";
    line += "[" + level + "] ";
    line += description;
    console.log( line );
  }

  var nestedResults = [];
  var startTest = function( display, description ) {
    if ( display ) {
      logCallback( 'note', indentLevel, description );
      ++indentLevel;
    }
    nestedResults.push( 'pass' );
  };

  var combineResults = function( result1, result2 ) {
    if ( result1 === 'pass' )
      return result2;
    else if ( result1 === 'warn' ) {
      if ( result2 === 'fail' )
        return 'fail';
      else
        return 'warn';
    }
    else return 'fail';
  }

  var counts = { pass: 0, warn: 0, fail: 0 };
  var insideKnownFailure = false;

  var stateResult = function( result, description ) {
    if ( result === 'fail' && insideKnownFailure )
      result = 'warn';
    ++counts[result];
    if ( nestedResults.length > 0 )
      nestedResults[nestedResults.length-1] = combineResults( nestedResults[nestedResults.length-1], result );
    logCallback( result, indentLevel, description );
  };

  var stateValue = function( result, description, value ) {
    if ( result === 'fail' && insideKnownFailure )
      result = 'warn';
    logCallback( result, indentLevel, "-> " + description + ": " + value );
  };

  var endTest = function( display, result, description ) {
    result = combineResults( result, nestedResults.pop() );
    if ( display )
      --indentLevel;
    stateResult( result, description );
  };

  var stateTotals = function() {
    var totalCount = counts['pass'] + counts['warn'] + counts['fail'];

    var result = 'pass';
    var resultDesc = "Total " + totalCount + " test(s)";
    if ( counts['fail'] > 0 ) {
      result = combineResults( result, 'fail' );
      resultDesc += ", " + counts['fail'] + " FAILURES";
    }
    if ( counts['warn'] > 0 ) {
      result = combineResults( result, 'warn' );
      resultDesc += ", " + counts['warn'] + " known failures";
    }
    if ( counts['fail'] == 0 && counts['warn'] == 0 ) {
      resultDesc += ", no failures";
    }
    logCallback( result, indentLevel, resultDesc );
  };

  var reportException = function( result, e ) {
    stateValue( result, "Exception", e );
  };

  var test = function( description, callback ) {
    startTest( true, description );
    var result = 'pass';
    try {
      callback.call();
    }
    catch ( e ) {
      result = 'fail';
      stateResult( result, "Unexpected exception thrown" );
      reportException( result, e );
    }
    endTest( true, result, description );
  };

  var knownFailure = function( callback ) {
    var oldInsideKnownFailure = insideKnownFailure;
    insideKnownFailure = true;
    callback.call();
    insideKnownFailure = oldInsideKnownFailure;
  };

  var expect = function( description, actualResult, expectedResult ) {
    if ( expectedResult === undefined )
      expectedResult = true;
    startTest( false, description );
    var result = 'pass';
    if ( actualResult !== expectedResult )
      result = 'fail'
    endTest( false, result, description );
    if ( result != 'pass' ) {
      stateValue( result, "Expected result", expectedResult );
      stateValue( result, "Actual result", actualResult );
    }
  };

  var expectDifferent = function( description, actualResult, unexpectedResult ) {
    startTest( false, description );
    var result = 'pass';
    if ( actualResult === unexpectedResult )
      result = 'fail';
    endTest( false, result, description );
    if ( result != 'pass' ) {
      stateFailedValue( description, "Actual result is unexpected", actualResult );
    }
  };

  var expectSuccess = function( description, callback ) {
    startTest( false, description );
    var result = 'pass';
    try {
      callback.call();
    }
    catch ( e ) {
      result = 'fail';
      stateResult( result, "Unexpected exception thrown" );
      reportException( result, e );
    }
    endTest( false, result, description );
  };

  var expectException = function( description, callback ) {
    startTest( false, description );
    var result = 'fail';
    try {
      callback.call();
    }
    catch ( e ) {
      result = 'pass';
    }
    endTest( false, result, description );
  };

  var done = function() {
    stateTotals();
  }

  return {
    test: test,
    expect: expect,
    expectDifferent: expectDifferent,
    expectSuccess: expectSuccess,
    expectException: expectException,
    knownFailure: knownFailure,
    done: done,
  };
};
