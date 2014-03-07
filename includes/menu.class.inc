<?php
function showMenu($begin_html = '', $end_html = '', $menu_array, $id = 'menu', $type = '', $space = true, $with_num = 0){
	// $menu_array 	- массив
	// $id 			- идентификатор
	// $type 		- тип
	// $spice 		- признак наличия пробелов

	if($type == 'li'){
		$txt .= '<ul class="'.$id.'">'.$begin_html;

	}elseif($type == 'td'){
		$txt .= '<table class="'.$id.'">'.$begin_html;

	}else{
        $txt .= '<div class="'.$id.'">'.$begin_html;
	}

	$num = 0;

	foreach($menu_array as $name => $lnk){
		$name_ = '';
		$cnt = '';

		if($num < $with_num){
			$num++;
			continue;
		}

		$show_submenu = false;
		$array = false;

		if($name == 'array'){
			$show_submenu = true;
			$array = $lnk;

            foreach($lnk as $name => $lnk){
            	$name = $name;
            	$lnk = $lnk;
            	break;
            }
		}

		if(strstr($name, '[')){
			$pos = strpos($name, '[');
			$pos2 = strpos($name, ']');
  			$cnt = substr($name, ($pos + 1), ($pos2 - $pos - 1));
  			if(!$name_){
  				$name_ = substr($name, 0, $pos);
  			}
		}

		if(strstr($name, '{')){
			$pos = strpos($name, '{');
			$pos2 = strpos($name, '}');
  			$width = substr($name, ($pos + 1), ($pos2 - $pos - 1));
  			if(!$name_){
  				$name_ = substr($name, 0, $pos);
  			}
		}

		if($name_){
			$name = $name_;
		}

		if($type == 'li' || 'td'){
			$txt .= '<'.$type.'';
		}

		if($_SERVER['REQUEST_URI'] == $lnk){
            if($type == 'li' || 'td'){
                $txt .= ' class="active"'.($width ? ' width="'.$width.'"' : '').'><b>'.$name.'</b>'.' '.$cnt;

			}else{
				$txt .= '<a>'.$name.'</a>'.' '.$cnt;
			}

		}elseif(@strstr($_SERVER['REQUEST_URI'], $lnk)){
			if($type == 'li' || 'td'){
                $txt .= ' class="active"'.($width ? ' width="'.$width.'"' : '').'><b><a href="'.$lnk.'">'.$name.'</a></b>'.' '.$cnt;

			}else{
				$txt .= '<a href="'.$lnk.'">'.$name.'</a>'.' '.$cnt;
			}

		}else{
			if($type == 'li' || 'td'){
                $txt .= ''.($width ? ' width="'.$width.'"' : '').'><a href="'.$lnk.'">'.$name.'</a>'.' '.$cnt;

			}else{
				$txt .= '<a href="'.$lnk.'">'.$name.'</a>'.' '.$cnt;
			}
		}

		if($show_submenu){
			$txt .= show_menu('', '', $array, $id.'_'.$num, $type, $space, 1);
		}

    	if($type == 'li' || 'td'){
			$txt .= ''.($space && count($menu_array) != ($num + 1) ? '<'.$type.' class="space"></'.$type.'>' : '').'</'.$type.'>';
		}

		$num++;
	}

	if($type == 'li'){
		$txt .= $end_html.'</ul>';

	}elseif($type == 'td'){
		$txt .= $end_html.'<!--<td width="100%"></td>--></table>';

	}else{
		$txt .= $end_html.'</div>';
	}

	return $txt;
}
?>