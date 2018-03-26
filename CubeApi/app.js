'use strict';
var express = require('express');
var path = require('path');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var cors = require('cors');
var fs = require('fs');
var uuid = require('uuid/v1');
const execFile = require('child_process').execFile;

var app = express();

app.use(cors({ origin: '*' }));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());

app.get('/', function (req, res) {
  res.send('Use POST /solve - you will need CubeConsole.exe in your working directory or PATH');
});

app.post('/solve', function (req, res) {
  if (!req.body || !req.body.scramble) {
    return res.sendStatus(400);
  }
  const scrambleFileName = 'scramble_' + uuid() + '.txt';
  fs.writeFile(scrambleFileName, req.body.scramble, (error) => {
    if (error) {
      res.json(error);
    }
    else {
      const child = execFile('CubeConsole.exe', ['scrambles=' + scrambleFileName], (error, stdout, stderr) => {
        if (error) {
          res.send(stderr);
        }
        else {
          var lines = stdout.split(/\r?\n/);
          // Line 0 is the scramble
          // Steps are from line 2 to n-2
          // Line n is the solution - starting with "Final solution"
          var solutionLineIndex = lines.findIndex((line) => {
            return line.startsWith('Final solution');
          });
          if (solutionLineIndex === -1) {
            res.send("No solution");
          }
          else {
            var solution = {
              solution: lines[solutionLineIndex].substr(lines[solutionLineIndex].indexOf(':') + 2), // Solution appears after a colon and a space in the final solution line.
              steps: []
            };
            for (var lineIndex = 2; lineIndex <= solutionLineIndex - 2; ++lineIndex) {
              if (lines[lineIndex].length > 0 && lines[lineIndex][0] !== ' ') { // Skip empty lines and indented lines (which are continuations of the previous line)
                var colonIndex = lines[lineIndex].indexOf(':');
                var openBracketIndex = lines[lineIndex].indexOf('[');
                // The step name is before the colon.
                var stepName = lines[lineIndex].substr(0, colonIndex);
                // The moves are after the colon but before the move count in square brackets. (Ignore colon and the leading and trailing space.)
                // This doesn't handle insertions yet.
                var movesText = lines[lineIndex].substr(colonIndex + 2, openBracketIndex - colonIndex - 3);
                solution.steps.push({
                  step: stepName,
                  moves: movesText.split(' ')
                });
              }
            }
            res.json(solution);
          }
        }
        fs.unlink(scrambleFileName);
      });
    }
  });
});

// Development error handler - will print stack trace
if (app.get('env') === 'development') {
  app.use(function (err, req, res, next) {
    res.status(err.status || 500);
    res.json({
      message: err.message,
      error: err
    });
  });
}

// Production error handler - no stack traces leaked to user
app.use(function (err, req, res, next) {
  res.status(err.status || 500);
  res.json({
    message: err.message,
    error: {}
  });
});

var debug = require('debug')('CubeApi');

app.set('port', process.env.PORT || 3000);

var server = app.listen(app.get('port'), function () {
  debug('Express server listening on port ' + server.address().port);
});
