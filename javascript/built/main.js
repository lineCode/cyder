var __reflect = (this && this.__reflect) || function (p, c, t) {
    p.__class__ = c, t ? t.push(c) : t = [c], p.__types__ = p.__types__ ? t.concat(p.__types__) : t;
};
var cyder;
(function (cyder) {
    function formatString(format) {
        var objects = new Array(arguments.length);
        for (var index = 0; index < arguments.length; index++) {
            objects[index] = arguments[index];
        }
        return objects.join(' ');
    }
    var Console = (function () {
        function Console(stdout, stderr) {
            this.stdout = stdout;
            this.stderr = stderr;
        }
        Console.prototype.log = function (message) {
            var optionalParams = [];
            for (var _i = 1; _i < arguments.length; _i++) {
                optionalParams[_i - 1] = arguments[_i];
            }
            var text = formatString.apply(this, arguments);
            this.stdout.write(text + "\n");
        };
        Console.prototype.assert = function (assertion, message) {
            var optionalParams = [];
            for (var _i = 2; _i < arguments.length; _i++) {
                optionalParams[_i - 2] = arguments[_i];
            }
            if (!assertion) {
                this.stderr.write(message + "\n");
            }
        };
        Console.prototype.warn = function (message) {
            var optionalParams = [];
            for (var _i = 1; _i < arguments.length; _i++) {
                optionalParams[_i - 1] = arguments[_i];
            }
            var text = formatString.apply(this, arguments);
            this.stderr.write(text + "\n");
        };
        Console.prototype.error = function (message) {
            var optionalParams = [];
            for (var _i = 1; _i < arguments.length; _i++) {
                optionalParams[_i - 1] = arguments[_i];
            }
            var text = formatString.apply(this, arguments);
            this.stderr.write(text + "\n");
        };
        return Console;
    }());
    cyder.Console = Console;
    __reflect(Console.prototype, "cyder.Console");
})(cyder || (cyder = {}));
this.console = new cyder.Console(stdout, stderr);
