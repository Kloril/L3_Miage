
window.onload = toc4;

function toc4(){
  var body = document.getElementsByTagName("body")[0];
  var elem = body.firstChild;
  var table = document.createElement("table");
  var actuel = elem.nextSibling;
  
  while(actuel.nextSibling!=null){
    var line = document.createElement("tr");
    var link = document.createElement("a");
    if(actuel.tagName == "H1" || actuel.tagName=="H2"){
      if(actuel.id === ""){
        actuel.setAttribute("id", actuel.innerHTML);
      }
      var id = actuel.id;
      link.setAttribute("href", "#" + id);
      listener(link,actuel);
      link.innerHTML= actuel.innerHTML;
      line.appendChild(link);
      table.appendChild(line);
    }
  
    
    actuel=actuel.nextSibling;
    
    
  }
  body.insertBefore(table,document.body.firstChild);
  table.setAttribute("border", 2);
}
function toc(){

  //var top = document.createElement("div");
  var elements = document.getElementsByTagName("H1");

  var body = document.getElementsByTagName("body")[0];
  //console.log(body);
//  body.appendChild(table);
  var table = document.createElement("table");

  for(var i = 0;i <elements.length;i++){
    var line = document.createElement("tr");
    var link = document.createElement("a");


    if(elements[i].id===""){
      elements[i].setAttribute("id", elements[i].innerHTML);
    }
    var id = elements[i].id;
    link.setAttribute("href","#" + id);
    listener(link);
    link.innerHTML= elements[i].innerHTML;
    line.appendChild(link);


    table.appendChild(line);

  }
  body.insertBefore(table,document.body.firstChild);
  table.setAttribute("border", 2);
}

function listener(element,tag){
  var background;
  if(tag.tagName=="H1"){
      background= "background-color:red";
  }
  else if (tag.tagName=="H2"){
      background= "background-color:green";
  }
  element.addEventListener("mouseover", function(){
    document.getElementById(this.getAttribute("href").slice(1)).setAttribute("style", background);
  })
  element.addEventListener("mouseleave", function(){
    document.getElementById(this.getAttribute("href").slice(1)).removeAttribute("style");
  })

}
