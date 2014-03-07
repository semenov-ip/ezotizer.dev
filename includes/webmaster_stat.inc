<?php
function show_stat(){
	global $base;
	global $var;

	$site_id = get_int_val('id');

	$min_dataadd = $base->exec('select min(date(dataadd)) as min
	from '.$var['base_tab_prefix'].'sites
	where user_id="'.$var['user_id'].'"');

	if($min_dataadd[0]['min'] == '')$min_dataadd[0]['min'] = date("d.m.Y", (strtotime($var['date']) - 604800));
	if(strtotime($min_dataadd[0]['min']) < (strtotime($var['datetime']) - 604800))$min_dataadd[0]['min'] = date("d.m.Y", (strtotime($var['date']) - 604800));

	$week_day = array(0=>6, 1=>0, 2=>1, 3=>2, 4=>3, 5=>4, 6=>5);

	$txt .= '<div class="stat_menu">';

	$txt .= '
	<div class="fast_select">
	 <a class="js_lnk" onclick="setdate(\''.date("d.m.Y", strtotime($var['date'])).'\', \''.date("d.m.Y", strtotime($var['date'])).'\')">Сегодня</a>
	 <a class="js_lnk" onclick="setdate(\''.date("d.m.Y", (strtotime($var['date']) - 86400)).'\', \''.date("d.m.Y", (strtotime($var['date']) - 86400)).'\')">Вчера</a>
	 <a class="js_lnk" onclick="setdate(\''.date("d.m.Y", (strtotime($var['date']) - (86400 * $week_day[date("w", strtotime($var['date']))]))).'\', \''.date("d.m.Y", strtotime($var['date'])).'\')">Текущая неделя</a>
	 <a class="js_lnk" onclick="setdate(\''.date("d.m.Y", (strtotime($var['date']) - (86400 * (date("j", strtotime($var['date'])) - 1)))).'\', \''.date("d.m.Y", strtotime($var['date'])).'\')">Текущий месяц</a>
	 <a class="js_lnk" onclick="setdate(\''.date("d.m.Y", (strtotime($var['date']) - (86400 * ($week_day[date("w", strtotime($var['date']))] + 7)))).'\', \''.date("d.m.Y", (strtotime($var['date']) - (86400 * ($week_day[date("w", strtotime($var['date']))] + 1)))).'\')">Прошлая неделя</a>
	 <a class="js_lnk" onclick="setdate(\''.date("d.m.Y", strtotime(date("Y", strtotime($var['date'])).'-'.(date("m", strtotime($var['date'])) - 1).'-01')).'\', \''.date("d.m.Y", (strtotime($var['date']) - (86400 * (date("j", strtotime($var['date'])))))).'\')">Прошлый месяц</a>
	</div>
 	<script language="javascript" type="text/javascript" src="/i/calendar_ru.js"></script>
	<form class="stat_form">
	 <table>
	  <tr>
	   <td><input id="begin_date" type="text" value="'.date("d.m.Y", strtotime($min_dataadd[0]['min'])).'" size="11" maxlength="10" class="inp" onfocus="this.select();lcs(this)" readonly onclick="event.cancelBubble=true;this.select();lcs(this)"></td>
	   <td>&mdash;</td>
	   <td><input id="end_date" type="text" value="'.date("d.m.Y", strtotime($var['datetime'])).'" size="11" maxlength="10" class="inp" onfocus="this.select();lcs(this)" readonly onclick="event.cancelBubble=true;this.select();lcs(this)"></td>
	   <td nowrap>
	    Сайт: <select id="id" onchange="change_id(this.value)">
         <option value="">Все</option>
    ';

    $site = $base->exec('select id, url
	from '.$var['base_tab_prefix'].'sites
	where user_id="'.$var['user_id'].'"
	order by dataadd desc');

	foreach($site as $site){
		if(strlen($site['url']) > 20)$site['url'] = substr($site['url'], 0, 20).'...';
		$txt .= '<option value="'.$site['id'].'"'.($site['id'] == $site_id ? ' selected' : '').'>'.$site['url'].' </option>';
	}

    $txt .= '
	    </select>
	   </td>
	   <td nowrap>
	    Показывать: <select onchange="change_period(this.value)" id="period">
	    <option value="days">По дням </option>
		<option value="weeks">По неделям </option>
		<option value="months">По месяцам </option>
		</select>
	   </td>
	   <td nowrap>
	    Показывать: <select onchange="change_field(this.value)" id="field">
	    <option value="view"'.($_COOKIE['user_site_stat_field'] == 'view' ? ' selected' : '').'>Показы </option>
		<option value="click"'.($_COOKIE['user_site_stat_field'] == 'click' ? ' selected' : '').'>Клики </option>
		<option value="ctr"'.($_COOKIE['user_site_stat_field'] == 'ctr' ? ' selected' : '').'>CTR </option>
		<option value="count_rur"'.($_COOKIE['user_site_stat_field'] == 'count_rur' ? ' selected' : '').'>Доход </option>
		</select>
	   </td>
	   <td style="vertical-align: middle;">
	    &nbsp;<img src="/i/refresh.gif" class="refresh_btn" width="16" height="16" align="absmiddle" alt="Обновить" title="Обновить" onclick="refrashGraph()">
	   </td>
	  </tr>
	 </table>
	</form>
	';
	$txt .= '</div>';

	$txt .= '<div class="graph">';
	$txt .= open_flash_chart_object('100%', 260, './?action=data&period=days&field='.($_COOKIE['user_site_stat_field'] ? $_COOKIE['user_site_stat_field'] : 'view').'&begindate='.date("d.m.Y", strtotime($min_dataadd[0]['min'])).'&enddate='.date("d.m.Y", strtotime($var['datetime'])).'');
    $txt .= '</div>';

	return $txt;
}

function show_stat_data(){
	global $var;

	srand((double)microtime()*1000000);
    $period = htmlspecialchars(trim(get_str_val('period')));

    if($period == 'days')show_stat_data_days();
    elseif($period == 'weeks')show_stat_data_weeks();
    elseif($period == 'months')show_stat_data_months();
}

function show_stat_data_days(){
	global $base;
	global $var;

	$site_id = get_int_val('id');
    $begindate = htmlspecialchars(trim(get_str_val('begindate')));
    $enddate = htmlspecialchars(trim(get_str_val('enddate')));
    $field = htmlspecialchars(trim(get_str_val('field')));
    @setcookie('user_site_stat_field', $field, time()+2592000, '/');

	$color = array(
		0=>'#80a033',1=>'#cc3399',2=>'#9933cc',3=>'#87421f',4=>'#82ca9c',
		5=>'#898989',6=>'#f26522',7=>'#f06eaa',8=>'#0092c8',9=>'#555555',
		10=>'',11=>'',12=>'',13=>'',14=>'',
		15=>'',16=>'',17=>'',18=>'',19=>''
	);

	$field_name = array('view'=>'Показы', 'click'=>'Клики', 'ctr'=>'CTR', 'count_rur'=>'Доход(руб)');

	$g = new graph();

	$site = $base->exec('select id, url
	from '.$var['base_tab_prefix'].'sites
	where user_id="'.$var['user_id'].'"
	'.($site_id ? 'and id="'.$site_id.'"' : '').'
	order by dataadd desc');

	if(count($site) > 0){

		$begin_day[0]['day'] = date("Y-m-d", strtotime($begindate));
		$t = ceil((strtotime($enddate) - strtotime($begin_day[0]['day'])) / 86400) + 1;

		for($i = 1; $i <= $t; $i++){
			$dt = strtotime($enddate) - (86400 * ($t -$i));
	    	$data_date[] = date("j", $dt).' '.iconv("windows-1251", "utf-8", num2month3(date("m", $dt))).'.`'.date("y", $dt).'';
		}

		$num = 0;
		foreach($site as $site){
			$data = array();

			for($i = 1; $i <= $t; $i++){
				$dt = strtotime($enddate) - (86400 * ($t -$i));

	    		$stat = $base->exec('select view, click, count_rur, dataadd
				from '.$var['base_tab_prefix'].'sites_stat
				where site_id = "'.$site['id'].'"
				and dataadd="'.date("Y-m-d", $dt).'"');

				$view = ($stat[0]['view'] ? $stat[0]['view'] : 0);
				$click = ($stat[0]['click'] ? $stat[0]['click'] : 0);
                $ctr = str_replace(",", ".", @sprintf("%.2f", (100 / $view) * $click));
				$count_rur = ($stat[0]['count_rur'] ? $stat[0]['count_rur'] : 0.00);

				if($field == 'ctr')$data[] = $ctr;
				elseif($field == 'view')$data[] = $view;
				elseif($field == 'click')$data[] = $click;
				elseif($field == 'count_rur')$data[] = $count_rur;
			}

			if(strlen($site['url']) > 15)$site['url'] = substr($site['url'], 0, 15).'...';
			$g->line_hollow(2, 3, $color[$num], iconv("windows-1251", "utf-8", $site['url']), 11);
			$g->set_data($data);

			foreach($data as $max_data){
        		if($max_value < $max_data){
        			$max_value = $max_data;
        		}
			}

			$num ++;
		}

		$max_value = ceil($max_value / 5);
		$max_value *= 5;

	}else{
		for($i = 1; $i <= 7; $i++){
			$dt = strtotime($var['date']) - (86400 * (7 -$i));
	    	$data_date[] = date("j", $dt).' '.iconv("windows-1251", "utf-8", num2month3(date("m", $dt))).' ('.$var['year'].')';
		}

		$data[] = '0';
		$max_value = 15;
		$g->set_data($data);
	}

	if($t > 10)$g->set_x_axis_steps(5);
	if($t > 50)$g->set_x_axis_steps(25);

	$g->set_x_labels($data_date);
	$g->set_y_max($max_value);

	if($t > 10)$g->set_x_label_style(10, '#343434', 0, 5, '#f5e1aa' );
	if($t > 50)$g->set_x_label_style(10, '#343434', 0, 25, '#f5e1aa' );

	$g->set_tool_tip(iconv("windows-1251", "utf-8", '#key#<br>'.$field_name[$field].': #val#<br>#x_label#'));
	$g->bg_colour = '#ffffff';
	echo $g->render();
}

function show_stat_data_weeks(){
	global $base;
	global $var;

	$site_id = get_int_val('id');
    $begindate = htmlspecialchars(trim(get_str_val('begindate')));
    $enddate = htmlspecialchars(trim(get_str_val('enddate')));
    $field = htmlspecialchars(trim(get_str_val('field')));
    @setcookie('user_site_stat_field', $field, time()+2592000, '/');

	$color = array(
		0=>'#80a033',1=>'#cc3399',2=>'#9933cc',3=>'#87421f',4=>'#82ca9c',
		5=>'#898989',6=>'#f26522',7=>'#f06eaa',8=>'#0092c8',9=>'#555555',
		10=>'',11=>'',12=>'',13=>'',14=>'',
		15=>'',16=>'',17=>'',18=>'',19=>''
	);

	$field_name = array('view'=>'Показы', 'click'=>'Клики', 'ctr'=>'CTR', 'count_rur'=>'Доход(руб)');

	$g = new graph();

	$site = $base->exec('select id, url
	from '.$var['base_tab_prefix'].'sites
	where user_id="'.$var['user_id'].'"
	'.($site_id ? 'and id="'.$site_id.'"' : '').'
	order by dataadd desc');

	if(count($site) > 0){

		$begindate  = date("Y-m-d", strtotime($begindate));
		$enddate = date("Y-m-d", strtotime($enddate));

		$tmp_week = '';
		for($i = strtotime($begindate); $i <= strtotime($enddate); $i += 86400){
			if(date("W", $i) != $tmp_week){
	    		if(date("m", $i) == '01' && date("W", $i) == '53'){
	    		    $data_date[] = date("W", $i).' '.iconv("windows-1251", "utf-8", 'нед.').'`'.date("y", ($i - (86400 * 30)));

	    		}elseif(date("m", $i) == '12' && date("W", $i) == '01'){
	    		    $data_date[] = date("W", $i).' '.iconv("windows-1251", "utf-8", 'нед.').'`'.date("y", ($i + (86400 * 30)));

	    		}else{
	    			$data_date[] = date("W", $i).' '.iconv("windows-1251", "utf-8", 'нед.').'`'.date("y", $i);
	    		}
	    		$tmp_week = date("W", $i);
	    	}
		}

		$num = 0;
		foreach($site as $site){
			$data = array();

			$view = 0;
			$click = 0;
			$count_rur = 0;
			$tmp_week = '';

			for($i = strtotime($begindate); $i <= strtotime($enddate); $i += 86400){
		    	if(date("W", $i) != $tmp_week){
		    		$stat = $base->exec('select view, click, count_rur, dataadd
					from '.$var['base_tab_prefix'].'sites_stat
					where site_id = "'.$site['id'].'"
					and week(dataadd, 3)="'.date("W", $i).'"');

					foreach($stat as $stat){
						$view += ($stat['view'] ? $stat['view'] : 0);
						$click += ($stat['click'] ? $stat['click'] : 0);
	   	            	$count_rur += ($stat['count_rur'] ? $stat['count_rur'] : 0.00);
	   	            }

		    		$ctr = str_replace(",", ".", @sprintf("%.2f", (100 / $view) * $click));

		    		if($field == 'ctr')$data[] = $ctr;
					elseif($field == 'view')$data[] = $view;
					elseif($field == 'click')$data[] = $click;
					elseif($field == 'count_rur')$data[] = str_replace(",", ".", @sprintf("%.2f", $count_rur));

		    		$view = 0;
					$click = 0;
					$count_rur = 0;
					$tmp_week = date("W", $i);
		    	}
			}

			if(strlen($site['url']) > 15)$site['url'] = substr($site['url'], 0, 15).'...';
			$g->line_hollow(2, 3, $color[$num], iconv("windows-1251", "utf-8", $site['url']), 11);
			$g->set_data($data);

			foreach($data as $max_data){
        		if($max_value < $max_data){
        			$max_value = $max_data;
        		}
			}

			$num ++;
		}

		$max_value = ceil($max_value / 5);
		$max_value *= 5;

	}else{
		for($i = 1; $i <= 7; $i++){
			$data_date[] = $var['week'] + ($i - 7).' '.iconv("windows-1251", "utf-8", 'нед.').' ('.$var['year'].')';
		}

		$data[] = '0';
		$max_value = 15;
		$g->set_data($data);
	}

	if($t > 10)$g->set_x_axis_steps(5);
	$g->set_x_labels($data_date);
	$g->set_y_max($max_value);
	if($t > 10)$g->set_x_label_style(10, '#343434', 0, 5, '#f5e1aa' );
	$g->set_tool_tip(iconv("windows-1251", "utf-8", '#key#<br>'.$field_name[$field].': #val#<br>#x_label#'));
	$g->bg_colour = '#ffffff';
	echo $g->render();
}

function show_stat_data_months(){
	global $base;
	global $var;

	$site_id = get_int_val('id');
    $begindate = htmlspecialchars(trim(get_str_val('begindate')));
    $enddate = htmlspecialchars(trim(get_str_val('enddate')));
    $field = htmlspecialchars(trim(get_str_val('field')));
    @setcookie('user_site_stat_field', $field, time()+2592000, '/');

	$color = array(
		0=>'#80a033',1=>'#cc3399',2=>'#9933cc',3=>'#87421f',4=>'#82ca9c',
		5=>'#898989',6=>'#f26522',7=>'#f06eaa',8=>'#0092c8',9=>'#555555',
		10=>'',11=>'',12=>'',13=>'',14=>'',
		15=>'',16=>'',17=>'',18=>'',19=>''
	);

	$field_name = array('view'=>'Показы', 'click'=>'Клики', 'ctr'=>'CTR', 'count_rur'=>'Доход(руб)');

	$g = new graph();

	$site = $base->exec('select id, url
	from '.$var['base_tab_prefix'].'sites
	where user_id="'.$var['user_id'].'"
	'.($site_id ? 'and id="'.$site_id.'"' : '').'
	order by dataadd desc');

	if(count($site) > 0){

		$begindate  = date("Y-m-d", strtotime($begindate));
		$enddate = date("Y-m-d", strtotime($enddate));

		$tmp_month = '';
		for($i = strtotime($begindate); $i <= strtotime($enddate); $i += 86400){
			if(date("m", $i) != $tmp_month){
	    		$data_date[] = iconv("windows-1251", "utf-8", num2month3(date("m", $i))).'.`'.date("y", $i);
	    		$tmp_month = date("m", $i);
	    	}
		}

		$num = 0;
		foreach($site as $site){
			$data = array();

			$view = 0;
			$click = 0;
			$count_rur = 0;
			$tmp_month = '';

			for($i = strtotime($begindate); $i <= strtotime($enddate); $i += 86400){
				if(date("m", $i) != $tmp_month){
		    		$stat = $base->exec('select view, click, count_rur, dataadd
					from '.$var['base_tab_prefix'].'sites_stat
					where site_id = "'.$site['id'].'"
					and month(dataadd)="'.date("m", $i).'"');

					foreach($stat as $stat){
						$view += ($stat['view'] ? $stat['view'] : 0);
						$click += ($stat['click'] ? $stat['click'] : 0);
	   		            $count_rur += ($stat['count_rur'] ? $stat['count_rur'] : 0.00);
	   		        }

		    		$ctr = str_replace(",", ".", @sprintf("%.2f", (100 / $view) * $click));

		    		if($field == 'ctr')$data[] = $ctr;
					elseif($field == 'view')$data[] = $view;
					elseif($field == 'click')$data[] = $click;
					elseif($field == 'count_rur')$data[] = str_replace(",", ".", @sprintf("%.2f", $count_rur));

		    		$view = 0;
					$click = 0;
					$count_rur = 0;
					$tmp_month = date("m", $i);
		    	}
			}

			if(strlen($site['url']) > 15)$site['url'] = substr($site['url'], 0, 15).'...';
			$g->line_hollow(2, 3, $color[$num], iconv("windows-1251", "utf-8", $site['url']), 11);
			$g->set_data($data);

			foreach($data as $max_data){
        		if($max_value < $max_data){
        			$max_value = $max_data;
        		}
			}

			$num ++;
		}

		$max_value = ceil($max_value / 5);
		$max_value *= 5;

	}else{
		for($i = 1; $i <= 2; $i++){
			$data_date[] = iconv("windows-1251", "utf-8", num2month3($var['month'] + ($i - 2))).'. ('.$var['year'].')';
		}

		$data[] = '0';
		$max_value = 15;
		$g->set_data($data);
	}

	if($t > 10)$g->set_x_axis_steps(5);
	$g->set_x_labels($data_date);
	$g->set_y_max($max_value);
	if($t > 10)$g->set_x_label_style(10, '#343434', 0, 5, '#f5e1aa' );
	$g->set_tool_tip(iconv("windows-1251", "utf-8", '#key#<br>'.$field_name[$field].': #val#<br>#x_label#'));
	$g->bg_colour = '#ffffff';
	echo $g->render();
}
?>