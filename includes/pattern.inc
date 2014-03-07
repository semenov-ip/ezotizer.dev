<?php
function show_pattern($field_array, $tag_delete = true){	// $field_array['pattern'] 					- скин
	// последующие $field_array['...']  	  	- поля скина для замены
	foreach($field_array as $key=>$value){    	if($key == "pattern"){        	$fp = @fopen($value, "r");

            if($fp){            	while(@!feof($fp)){            		$pattern .= @fgets($fp, 1024);
            	}
            }
    	}elseif($key == "pattern_html"){            $pattern = $value;
    	}else{        	$pattern = str_replace("{-".$key."-}", $value, $pattern);    	}	}

	if($tag_delete){		$pattern = preg_replace("({-([a-z_]+)-})", "", $pattern);
	}

	return $pattern;}
?>