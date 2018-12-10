<?php
$q=$_GET["q"];
$i=0;
if($dir=opendir($q)){
    while(false != ($file = readdir($dir))){
        if(filetype($file)=="dir"){
            $tab[$i]=[$file,"dir"];
            $i++;
        }
       else {
            $tab[$i]=[$file,"file"];
            $i++;
       }
        
    }
   
}
closedir($dir);
echo json_encode($tab);

?>