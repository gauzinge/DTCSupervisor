//from: https://root.cern.ch/js/3.0/demo/demo.htm?layout=grid3x2

var painter = null;
var mdi = null;
var cnt = 0;
var drawopt = null;
var addr = null;

function updateGUI() {

   // here set of generated json files are used
   // One could specify addres of running THttpServer like http://localhost:8080/Canvases/c1/root.json.gz?compact=3
   // Or one can create root.json file in the application and place it on the webserver 
   // "dummy=xxxx" parameter used to prevent browser cashing
   // To run demo, one should generate rootXX.json files using demo.C macro
   
   //var request_addr = "root" + (cnt%20) + ".json?dummy=" + cnt;
   //var request_addr = "http://cmsuptrackerdaq.cern.ch:8080/Canvases/Occupancy/root.json?dummy=" + cnt;
    var request_addr = "http://cmsuptrackerdaq.cern.ch:8080/Calibrations/CommissioningCycle.root";
    ///Occupancy/root.json?dummy=" + cnt;
   cnt++;
   
   if (addr!=null) request_addr = addr; 

   var req = JSROOT.NewHttpRequest(request_addr, 'object', function(histo) {
      if (!histo) {
         if (!painter && !mdi) $('#drawing').append("<h3>Can not get " + request_addr + " from the server</h3>"); 
         return;
      }
      
      if (mdi!=null) {
         mdi.Draw('abstract_item_name_' + cnt, histo, drawopt); 
      } else { 
         painter = JSROOT.redraw('drawing', histo, drawopt);
      } 
       console.log(histo)
    });
    
    req.send(null); 
}

function startGUI() {
   $('html').css('height','100%');
   $('body').css('min-height','100%').css('margin','0px').css("overflow", "hidden");

   var monitor = JSROOT.GetUrlOption("monitoring");
   if ((monitor == "") || (monitor==null)) 
      monitor = 1000;
   else
      monitor = parseInt(monitor);
      
   drawopt = JSROOT.GetUrlOption("opt");
   
   addr = JSROOT.GetUrlOption("addr");
   
   var layout = JSROOT.GetUrlOption("layout");
   if (layout!=null) mdi = new JSROOT.GridDisplay('drawing', layout);   

   setInterval(updateGUI, monitor);
   
   //JSROOT.RegisterForResize(function() { if (painter) painter.CheckResize(); if (mdi) mdi.CheckResize(); });
}

function addLoadEvent(func) { 
  var oldonload = window.onload; 
  if (typeof window.onload != 'function') { 
    window.onload = func; 
  } else { 
    window.onload = function() { 
      if (oldonload) { 
        oldonload(); 
      } 
      func(); 
    } 
  } 
} 

addLoadEvent(startGUI); 
