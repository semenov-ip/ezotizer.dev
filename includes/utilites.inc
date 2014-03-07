<?php
function float_to_mysql($num){
	$number = str_replace(",", ".", @sprintf("%.2f", $num));
	return $number;
}

function date2str($date){

	if($date != '0000-00-00 00:00:00'){
		$dt = strtotime($date);

		if(date("j.m.Y", $dt) == date("j.m.Y", strtotime($var['date']))){
			$txt = '�������'.(date("H:i", $dt) != '00:00' ? '&nbsp;�&nbsp;'.date("H:i", $dt) : '');

		}elseif(date("j.m.Y", $dt) == date("j.m.Y", (strtotime($var['date']) - 86400))){
			$txt = '�����'.(date("H:i", $dt) != '00:00' ? '&nbsp;�&nbsp;'.date("H:i", $dt) : '');

	  	}else{
			$txt = date("j", $dt).' '.num2month3(date("m", $dt)).' '.date("Y", $dt);
		}

	}else{
		$txt = '&mdash;';
	}

	return $txt;

function num2month($num){
		case 2: $txt.="�������"; break;
		case 3: $txt.="�����"; break;
		case 4: $txt.="������"; break;
		case 5: $txt.="���"; break;
		case 6: $txt.="����"; break;
		case 7: $txt.="����"; break;
		case 8: $txt.="�������"; break;
		case 9: $txt.="��������"; break;
		case 10: $txt.="�������"; break;
		case 11: $txt.="������"; break;
		case 12: $txt.="�������"; break;
		}

	}

function num2month2($num){
	switch ($num){
		case 1: $txt.="������"; break;
		case 2: $txt.="�������"; break;
		case 3: $txt.="����"; break;
		case 4: $txt.="������"; break;
		case 5: $txt.="���"; break;
		case 6: $txt.="����"; break;
		case 7: $txt.="����"; break;
		case 8: $txt.="������"; break;
		case 9: $txt.="��������"; break;
		case 10: $txt.="�������"; break;
		case 11: $txt.="������"; break;
		case 12: $txt.="�������"; break;
	}

	return $txt;
}

function num2month3($num){
	switch ($num){
		case 1: $txt.="���."; break;
		case 2: $txt.="���."; break;
		case 3: $txt.="���."; break;
		case 4: $txt.="���."; break;
		case 5: $txt.="���"; break;
		case 6: $txt.="���."; break;
		case 7: $txt.="���."; break;
		case 8: $txt.="���."; break;
		case 9: $txt.="����."; break;
		case 10: $txt.="���."; break;
		case 11: $txt.="����."; break;
		case 12: $txt.="���."; break;
	}

	return $txt;
}

function get_str_val($str){
}

function post_str_val($str){
	if($_POST[$str]){return trim($_POST[$str]);}else{return false;}
}

function get_int_val($num){
	if($_GET[$num]){
		if(is_numeric(trim($_GET[$num])) && !strstr(trim($_GET[$num]), ".") && !strstr(trim($_GET[$num]), ",")){
		}else{
			return false;

	}else{
}

function post_int_val($num){
	if($_POST[$num]){
		if(is_numeric(trim($_POST[$num])) && !strstr(trim($_POST[$num]), ".") && !strstr(trim($_POST[$num]), ",")){
			return trim($_POST[$num]);

		}else{
			return false;
		}
	}else{
		return false;
	}
}

function post_float_val($num){
	$_POST[$num] = str_replace(",", ".", $_POST[$num]);

	if($_POST[$num]){
		if(is_numeric(trim($_POST[$num]))){
			return trim($_POST[$num]);

		}else{
			return false;
		}
	}else{
		return false;
	}
}

function get_float_val($num){
	$_GET[$num] = str_replace(",", ".", $_GET[$num]);

	if($_GET[$num]){
		if(is_numeric(trim($_GET[$num]))){
			return trim($_GET[$num]);

		}else{
			return false;
		}
	}else{
		return false;
	}
}

function return_word_end($num, $word, $end1, $end2, $end3){
	$len = strlen($num);
	if($len == 2 and $num < 21){
		$end_word = $end3;
	}else{
		$num = substr($num, ($len-1));
		switch ($num){
			case '0':{
				$end_word = $end3;
				break;
			}
			case '1':{
        	    $end_word = $end1;
				break;
			}
			case '2':{
            	$end_word = $end2;
				break;
			}
			case '3':{
            	$end_word = $end2;
				break;
			}
			case '4':{
            	$end_word = $end2;
				break;
			}
			case '5':{
            	$end_word = $end3;
				break;
			}
			case '6':{
            	$end_word = $end3;
				break;
			}
			case '7':{
            	$end_word = $end3;
				break;
			}
			case '8':{
            	$end_word = $end3;
				break;
			}
			case '9':{
            	$end_word = $end3;
				break;
			}
		}
	}
	return $word.$end_word;
}

function num2word_end($num, $word, $end1, $end2, $end3){
	$len = strlen($num);
	if($len == 2 and $num < 21){
		$end_word = $end3;
	}else{
		$num = substr($num, ($len-1));
		switch ($num){
			case '0':{
				$end_word = $end3;
				break;
			}
			case '1':{
        	    $end_word = $end1;
				break;
			}
			case '2':{
            	$end_word = $end2;
				break;
			}
			case '3':{
            	$end_word = $end2;
				break;
			}
			case '4':{
            	$end_word = $end2;
				break;
			}
			case '5':{
            	$end_word = $end3;
				break;
			}
			case '6':{
            	$end_word = $end3;
				break;
			}
			case '7':{
            	$end_word = $end3;
				break;
			}
			case '8':{
            	$end_word = $end3;
				break;
			}
			case '9':{
            	$end_word = $end3;
				break;
			}
		}
	}
	return $word.$end_word;
}

function pages_list($goods, $start_link, $end_link, $page_lnk = '?page', $on_page, $curr_page){

	$txt .= '<table class="pages">';

	/*$txt .= '<td>';

	if($curr_page <= 1){
	}else{

	$txt .= '</td>';*/

    $txt .= '<td>��������: </td>';

	if(($curr_page-4) > 1){
		$txt .= '<td class="n not-active"><a href="'.$start_link.$page_lnk.'='.($curr_page-5).$end_link.'">...</a></td><td class="space"></td>';
	}

	for($i = ($curr_page <= 5 ? 1 : $curr_page-4); $i < $curr_page + 5; $i++){

		if($i == $curr_page){
		}else{
	}

	if($pages >= $i){
	}

    $txt .= '<td>';

	if($curr_page >= $pages){

	}else{

	$txt .= '</td>';

	$txt .= '</table>';
	return $txt;

function pages_list_lite($goods, $start_link, $end_link, $var = '?page', $on_page, $curr_page){
	$pages = ceil($goods/$on_page);

	$txt .= '<span id="pages_lite">';

    $txt .= '<span id="nums">';

	for($i=($curr_page<=5?1:$curr_page-4); $i<$curr_page+5; $i++){
		if($i > $pages)break;
		$txt .= '<a href="'.$start_link.$var.'='.$i.$end_link.'">'.$i.'</a>';
	}

	if($pages >= $i){
		$txt .= '<a href="'.$start_link.$var.'='.($pages).$end_link.'" title="���������">>></a>';
	}

    $txt .= '</span>';
	$txt .= '</span>';

	return $txt;
}

function numtostr($num){
	switch ($num){
		case 1: $txt.="����"; break;
		case 2: $txt.="����"; break;
		case 3: $txt.="����"; break;
		case 4: $txt.="�������"; break;
		case 5: $txt.="����"; break;
		case 6: $txt.="�����"; break;
		case 7: $txt.="����"; break;
		case 8: $txt.="������"; break;
		case 9: $txt.="������"; break;
		case 10: $txt.="������"; break;
		case 11: $txt.="����������"; break;
		case 12: $txt.="����������"; break;
	}

	return $txt;
}
?>