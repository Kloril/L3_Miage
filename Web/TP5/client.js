
function ask(str) {
    xmlhttp=new XMLHttpRequest();      
    xmlhttp.onreadystatechange=function() {
       if (xmlhttp.readyState==4 && xmlhttp.status==200) {
           var decoded=JSON.parse(this.responseText);
          var table = document.createElement("table");
           var i=0;
           while(decoded[i]!=null){
            var line = document.createElement("tr");
           
            line.innerHTML = decoded[i][0] + "</br>";
            if(decoded[i][1]=="dir"){
                line.style.color= "red";
            }
            
            table.appendChild(line);
            i++;
           }
           document.body.appendChild(table);
           //document.getElementById("rep").innerHTML=table.innerHTML;
           //xmlhttp.responseText;
       }
    }
    xmlhttp.open("GET","serveur.php?q=" + str,true);
    xmlhttp.send();
    //console.log(decoded);
 }

 function print(){
   //  console.log(decoded);
 }