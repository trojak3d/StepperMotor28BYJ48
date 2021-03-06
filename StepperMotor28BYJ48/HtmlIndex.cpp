#include "HtmlIndex.h"

static const char html_index[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <title>ESP Input Form</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <style>
        body {
            font-family: Calibri;
        }

        .container {
            font-size: 16px;
            max-width: 400px;
            margin-right: auto;
            margin-left: auto;
            border: 1px solid lightgrey;
            border-radius: 2px;
        }

        .form {
            margin: 10px;
        }

        h1 {
            margin-top: 0;
            text-align: center;
        }

        label, input {
            height: 24px;
            align-self: center;
        }

        input {
            border: 1px solid lightgrey;
        }

        .form-field {
            display: flex;
            margin: 10px 0;
            flex-wrap: wrap;
        }

            .form-field label {
                width: 110px;
                align-self: center;
            }

            .form-field input {
                flex-basis: 0;
                flex-grow: 1;
                min-width: 100px;
                border-radius: 2px;
            }

            .form-field small {
                width: 100%;
            }

        button {
            width: 100%;
            height: 30px;
            border: 0;
            font-weight: 600;
            border-radius: 2px;
        }

            button:hover {
                color: white;
            }

            button.update {
                background-color: lightgrey;
            }

                button.update:hover {
                    background-color: grey;
                }

            button.start {
                background-color: lightgreen;
            }

                button.start:hover {
                    background-color: mediumseagreen;
                }

            button.pause {
                background-color: orange;
            }

                button.pause:hover {
                    background-color: darkorange;
                }

            button.stop {
                background-color: indianred;
            }

                button.stop:hover {
                    background-color: darkred;
                }

            button.reset {
                background-color: orange;
            }

                button.reset:hover {
                    background-color: darkorange;
                }

        .radio-group {
            width: 100%;
        }

            .radio-group input {
                min-width: 0;
                height: unset;
            }

            .radio-group > div {
                height: 24px;
            }

        .step-buttons {
            display: flex;
            flex-wrap: nowrap;
            justify-content: space-between;
        }

            .step-buttons > * {
                flex-basis: 0;
                flex-grow: 1;
            }

                .step-buttons > *:not(:first-child) {
                    margin-left: 5px;
                }

                .step-buttons > *:not(:last-child) {
                    margin-right: 5px;
                }

        .icon {
            font-family: Quivira, Symbola, EversonMono;
            font-weight: unset;
        }
    </style>
</head>
<body>
    <div class="container">
        <form class="form" action="/motor">
            <h1>Stepper Motor</h1>
            <div class="form-field">
                <label>StepDelay</label>
                <input type="number" value="[stepDelay]" name="step-delay">
                <small>StepDelay is defined by the delay interval between steps in ms</small>
                <small>2ms = fastest</small>
            </div>

            <div class="form-field">
                <label>Step Angle</label>
                <input type="number" value="[stepAngle]" step="5.625" name="step-angle">
                <small>Angle of step between pause intervals in 5.625&deg; intervals</small>
                <small>5.625&deg; = smallest</small>
            </div>

            <div class="form-field">
                <label>Pause Interval</label>
                <input type="number" value="[pauseInterval]" step="0.1" name="pause-interval">
                <small>Pause interval in seconds</small>
                <small>0s = no pause</small>
            </div>

            <div class="form-field">
                <label>Total Angle</label>
                <input type="number" value="[totalAngle]" name="total-angle">
                <small>Total angle of rotation</small>
                <small>360&deg; = full circle</small>
            </div>

            <div class="form-field">
                <label>Rotations</label>
                <input type="number" value="[rotations]" name="rotations">
                <small>Number of total angle rotations</small>
            </div>

            <div class="form-field">
                <label>Direction</label>
                <div class="radio-group">
                    <div>
                        <input type="radio" id="forward" name="forward" value="true" [forward]>
                        <label for="huey">Forward</label>
                    </div>

                    <div>
                        <input type="radio" id="backward" name="forward" value="false" [backward]>
                        <label for="dewey">Backward</label>
                    </div>
                </div>
            </div>

            <button type="submit" class="update">Update</button>
        </form>
        <form class="form" action="/reset">
            <button type="submit" class="reset">Reset</button>
        </form>
        <div class="step-buttons form">
            <form action="/start" class="step-buttons">
                <input type="number" value="[delay]" name="delay" placeholder="delay" style=" width: 50%;">
                <button type="submit" class="start icon">&#x25B6;&#xfe0e;</button>
            </form>
            <div class="step-buttons">
                <form action="/pause">
                    <button type="submit" class="pause icon">&#x23F8;&#xfe0e;</button>
                </form>
                <form action="/stop">
                    <button type="submit" class="stop icon">&#x23F9;&#xfe0e;</button>
                </form>
            </div>
        </div>
        <div class="step-buttons form">
            <form action="/step-backward">
                <button type="submit" class="reset icon">&#x23EA;&#xfe0e;</button>
            </form>
            <form action="/step-forward">
                <button type="submit" class="reset icon">&#x23E9;&#xfe0e;</button>
            </form>
        </div>
    </div>
</body>
</html>
)rawliteral";

String GetHtml(int stepDelay, float stepAngle, float pauseInterval, int totalAngle, int rotations, int startDelay, bool isForward) {
    String s = html_index;
    s.replace("[stepDelay]", String(stepDelay));
    s.replace("[stepAngle]", String(stepAngle));
    s.replace("[pauseInterval]", String(pauseInterval));
    s.replace("[totalAngle]", String(totalAngle));
    s.replace("[rotations]", String(rotations));
    s.replace("[delay]", String(startDelay));

    if (isForward) {
        s.replace("[forward]", "checked");
        s.replace("[backward]", "");
    } else {
        s.replace("[forward]", "");
        s.replace("[backward]", "checked");
    }

    return s;
}