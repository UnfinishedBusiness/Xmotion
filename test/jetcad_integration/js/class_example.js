var JetCad;
(function (JetCad) {
  var test_text = "The text!";
    function test()
    {
      console.log(test_text);
    }
    JetCad.test = test;
})(JetCad || (JetCad = {}));
module.exports = JetCad;
