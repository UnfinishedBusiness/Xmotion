// foo/bar.js
var text = 'Hello world!';     // not visible outside the module
exports.hello = function () {
    print(text);
};
