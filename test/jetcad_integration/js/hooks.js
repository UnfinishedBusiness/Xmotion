Duktape.modSearch = function (id, require, exports, module) {
  //print("Loading Module - " + id);
  if (id === 'makerjs')
  {
    return get_file("js/maker.js");  // undefined: no ECMAScript source
  }
  if (id === 'JetCad')
  {
    return get_file("js/JetCad.js");  // undefined: no ECMAScript source
  }
  if (id === 'clone')
  {
    return get_file("js/clone.js");  // undefined: no ECMAScript source
  }
  if (id === 'graham_scan')
  {
    return get_file("js/graham_scan.js");  // undefined: no ECMAScript source
  }
  throw new Error('cannot find module: ' + id);
};
