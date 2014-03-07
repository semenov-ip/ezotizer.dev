<?php
function float_to_mysql($num){
	$number = str_replace(",", ".", @sprintf("%.2f", $num));
	return $number;
}

function date2str($date){	global $var;

	if($date != '0000-00-00 00:00:00'){
		$dt = strtotime($date);

		if(date("j.m.Y", $dt) == date("j.m.Y", strtotime($var['date']))){
			$txt = 'сегодня'.(date("H:i", $dt) != '00:00' ? '&nbsp;в&nbsp;'.date("H:i", $dt) : '');

		}elseif(date("j.m.Y", $dt) == date("j.m.Y", (strtotime($var['date']) - 86400))){
			$txt = 'вчера'.(date("H:i", $dt) != '00:00' ? '&nbsp;в&nbsp;'.date("H:i", $dt) : '');

	  	}else{
			$txt = date("j", $dt).' '.num2month3(date("m", $dt)).' '.date("Y", $dt);
		}

	}else{
		$txt = '&mdash;';
	}

	return $txt;}

function num2month($num){	switch ($num){		case 1: $txt.="января"; break;
		case 2: $txt.="Февраля"; break;
		case 3: $txt.="марта"; break;
		case 4: $txt.="апреля"; break;
		case 5: $txt.="мая"; break;
		case 6: $txt.="июня"; break;
		case 7: $txt.="июля"; break;
		case 8: $txt.="августа"; break;
		case 9: $txt.="сентября"; break;
		case 10: $txt.="октября"; break;
		case 11: $txt.="ноября"; break;
		case 12: $txt.="декабря"; break;
		}
	return $txt;
	}

function num2month2($num){
	switch ($num){
		case 1: $txt.="Январь"; break;
		case 2: $txt.="Февраль"; break;
		case 3: $txt.="Март"; break;
		case 4: $txt.="Апрель"; break;
		case 5: $txt.="Май"; break;
		case 6: $txt.="Июнь"; break;
		case 7: $txt.="Июль"; break;
		case 8: $txt.="Август"; break;
		case 9: $txt.="Сентябрь"; break;
		case 10: $txt.="Октябрь"; break;
		case 11: $txt.="Ноябрь"; break;
		case 12: $txt.="Декабрь"; break;
	}

	return $txt;
}

function num2month3($num){
	switch ($num){
		case 1: $txt.="янв."; break;
		case 2: $txt.="фев."; break;
		case 3: $txt.="мар."; break;
		case 4: $txt.="апр."; break;
		case 5: $txt.="май"; break;
		case 6: $txt.="июн."; break;
		case 7: $txt.="июл."; break;
		case 8: $txt.="авг."; break;
		case 9: $txt.="сент."; break;
		case 10: $txt.="окт."; break;
		case 11: $txt.="нояб."; break;
		case 12: $txt.="дек."; break;
	}

	return $txt;
}

function get_str_val($str){	if($_GET[$str]){return trim($_GET[$str]);}else{return false;}
}

function post_str_val($str){
	if($_POST[$str]){return trim($_POST[$str]);}else{return false;}
}

function get_int_val($num){
	if($_GET[$num]){
		if(is_numeric(trim($_GET[$num])) && !strstr(trim($_GET[$num]), ".") && !strstr(trim($_GET[$num]), ",")){			return trim($_GET[$num]);
		}else{
			return false;		}

	}else{		return false;	}
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

function pages_list($goods, $start_link, $end_link, $page_lnk = '?page', $on_page, $curr_page){	$pages = ceil($goods/$on_page);

	$txt .= '<table class="pages">';

	/*$txt .= '<td>';

	if($curr_page <= 1){		$txt .= '<a class="hide">&larr;&nbsp;предыдущая</a>';
	}else{		$txt .= '<a href="'.$start_link.$page_lnk.'='.($curr_page-1).$end_link.'" class="arr">&larr;&nbsp;предыдущая</a>';	}

	$txt .= '</td>';*/

    $txt .= '<td>Страницы: </td>';

	if(($curr_page-4) > 1){
		$txt .= '<td class="n not-active"><a href="'.$start_link.$page_lnk.'='.($curr_page-5).$end_link.'">...</a></td><td class="space"></td>';
	}

	for($i = ($curr_page <= 5 ? 1 : $curr_page-4); $i < $curr_page + 5; $i++){		if($i > $pages)break;

		if($i == $curr_page){			$txt .= '<td class="n active b">'.$i.'</td>';
		}else{			$txt .= '<td class="n not-active"><a href="'.$start_link.$page_lnk.'='.$i.$end_link.'">'.$i.'</a></td><td class="space"></td>';		}
	}

	if($pages >= $i){		$txt .= '<td class="n not-active"><a href="'.$start_link.$page_lnk.'='.($i).$end_link.'">...</a></td><td class="space"></td>';
	}

    $txt .= '<td>';

	if($curr_page >= $pages){		$txt .= '<a class="hide">следующая&nbsp;&rarr;</a>';

	}else{		$txt .= '<a href="'.$start_link.$page_lnk.'='.($curr_page+1).$end_link.'" class="arr">следующая&nbsp;&rarr;</a> ';	}

	$txt .= '</td>';

	$txt .= '</table>';
	return $txt;}

function pages_list_lite($goods, $start_link, $end_link, $var = '?page', $on_page, $curr_page){
	$pages = ceil($goods/$on_page);

	$txt .= '<span id="pages_lite">';

    $txt .= '<span id="nums">';

	for($i=($curr_page<=5?1:$curr_page-4); $i<$curr_page+5; $i++){
		if($i > $pages)break;
		$txt .= '<a href="'.$start_link.$var.'='.$i.$end_link.'">'.$i.'</a>';
	}

	if($pages >= $i){
		$txt .= '<a href="'.$start_link.$var.'='.($pages).$end_link.'" title="Последняя">>></a>';
	}

    $txt .= '</span>';
	$txt .= '</span>';

	return $txt;
}

function numtostr($num){
	switch ($num){
		case 1: $txt.="Одна"; break;
		case 2: $txt.="Двух"; break;
		case 3: $txt.="Трех"; break;
		case 4: $txt.="Четырех"; break;
		case 5: $txt.="Пяти"; break;
		case 6: $txt.="Шести"; break;
		case 7: $txt.="Семи"; break;
		case 8: $txt.="Восьми"; break;
		case 9: $txt.="Девяти"; break;
		case 10: $txt.="Десяти"; break;
		case 11: $txt.="Одинадцати"; break;
		case 12: $txt.="Двенадцати"; break;
	}

	return $txt;
}
?>