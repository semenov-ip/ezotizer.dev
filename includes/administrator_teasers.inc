<?php
function show_administrator_teasers(){
	global $base;
	global $var;

	$status = get_str_val('status');

	if($_GET['status'] == ''){
		@header('location: ./?action=teasers&status=2');
		exit;
	}

	$section = $base->exec('select id, name
	from '.$var['base_tab_prefix'].'sections');

	foreach($section as $section){
		$section_arr[$section['id']] = $section['name'];
	}

	$status_arr = array(
		0 => 'новый',
		1 => 'на модерации',
		2 => 'идут показы',
		3 => 'показы<br/>приостановлены',
		4 => 'заблокирован'
	);

	$teaser_active_cnt = $base->exec('select count(*) from '.$var['base_tab_prefix'].'teasers
	where status = "2"');

	$teaser_stoped_cnt = $base->exec('select count(*) from '.$var['base_tab_prefix'].'teasers
	where status = "3"');

	$teaser_blocked_cnt = $base->exec('select count(*) from '.$var['base_tab_prefix'].'teasers
	where status = "4"');
	
	  $min_dataadd = $base->exec('select min(date(dataadd)) as min
    from '.$var['base_tab_prefix'].'campaigns
    where user_id="'.$var['user_id'].'"');
  
  if($min_dataadd[0]['min'] == '') $min_dataadd[0]['min'] = date("d.m.Y", (strtotime($var['date']) - 604800));
  
  $week_day = array(0=>6, 1=>0, 2=>1, 3=>2, 4=>3, 5=>4, 6=>5);
  
  $txt .= '
    <div class="fast_select position_select">
        <a class="js_lnk" onclick="setdate(\''.date("d.m.Y", strtotime($var['date'])).'\', \''.date("d.m.Y", strtotime($var['date'])).'\');">Сегодня</a>
        <div class="space">&nbsp;</div>
        <a class="js_lnk" onclick="setdate(\''.date("d.m.Y", (strtotime($var['date']) - 86400)).'\', \''.date("d.m.Y", (strtotime($var['date']) - 86400)).'\')">Вчера</a>
        <div class="space">&nbsp;</div>
        <a class="js_lnk" onclick="setdate(\''.date(    "d.m.Y", (strtotime($var['date']) - (86400 * $week_day[date("w", strtotime($var['date']))]))   ).'\', \''.date("d.m.Y", strtotime($var['date'])).'\')">Текущая неделя</a>
        <div class="space">&nbsp;</div>
        <a class="js_lnk" onclick="setdate(\''.date("d.m.Y", (strtotime($var['date']) - (86400 * (date("j", strtotime($var['date'])) - 1)))).'\', \''.date("d.m.Y", strtotime($var['date'])).'\')">Текущий месяц</a>
        <div class="space">&nbsp;</div>
        <a class="js_lnk" onclick="setdate(\''.date("d.m.Y", (strtotime($var['date']) - (86400 * ($week_day[date("w", strtotime($var['date']))] + 7)))).'\', \''.date("d.m.Y", (strtotime($var['date']) - (86400 * ($week_day[date("w", strtotime($var['date']))] + 1)))).'\')">Прошлая неделя</a>
        <div class="space">&nbsp;</div>
        <a class="js_lnk" onclick="setdate(\''.date("d.m.Y", strtotime(date("Y", strtotime($var['date'])).'-'.(date("m", strtotime($var['date'])) - 1).'-01')).'\', \''.date("d.m.Y", (strtotime($var['date']) - (86400 * (date("j", strtotime($var['date'])))))).'\')">Прошлый месяц</a>
    </div>
    <script language="javascript" type="text/javascript" src="/i/calendar_ru.js"></script>
    <form id="myform" class="stat_form margin_top_80" method="POST">
      <table width=40% align="right" style="margin-bottom:20px;">
        <tr>
          <td width=15%>&nbsp;</td>
          <td width=30%>Фильтр по дататм: </td>
          <td width=15%><input type="text" name="date_start" class="datepicker" value="'.date("d.m.Y", strtotime($min_dataadd[0]['min'])).'" size="14" maxlength="10"></td>
          <td width=5% align=center>&mdash;</td>
          <td width=15%><input type="text" name="date_end" class="datepicker" value="'.date("d.m.Y", strtotime($var['datetime'])).'" size="14" maxlength="10"></td>
          <td width=20% align="right"><input type="submit" name="statisic" value="Применить"></td>
        </tr>
      </table>
    </form>';
	
	$txt .= showMenu('', '',
		array(
		'Активные[('.$teaser_active_cnt[0][0].')]' => '/administrator/?action=teasers&status=2',
		'Приостановленные[('.$teaser_stoped_cnt[0][0].')]' => '/administrator/?action=teasers&status=3',
		'Заблокированные[('.$teaser_blocked_cnt[0][0].')]' => '/administrator/?action=teasers&status=4',
		), 'navigation_sub_menu', 'td', false
	);

	$txt .= '
	<table class="list">
	 <tr class="head">
	  <td>Пользователь</td>
	  <td>Изображение</td>
	  <td>Текст</td>
	  <td>Тематика</td>
	  <td>Показы</td>
	  <td>Клики</td>
	  <td>CTR</td>
	  <td>Расход</td>
	  <td></td>
	 </tr>
	';

	$teaser = $base->exec('select id, user_id, section_id, image, url, text, status
	from '.$var['base_tab_prefix'].'teasers
	where status = "'.$status.'"
	order by dataadd desc');

	$campaignStatistics = campaignsStatExtratc($teaser, 'teasers_stat', 'teaser_id');
	
    $view_cnt = 0;
	$click_cnt = 0;

	if(count($campaignStatistics) > 0){
		$num = 0;

		foreach($campaignStatistics as $teaser){
			$view = $teaser['view'];
            $click = $teaser['click'];
            $ctr = $teaser['ctr'];
            $count_rur = $teaser['count_rur'];

			$user = $base->exec('select id, email, hash
			from '.$var['base_tab_prefix'].'users
			where id = "'.$teaser['user_id'].'"');

			if(strlen($user[0]['email']) > 10){
				$user_email = substr($user[0]['email'], 0, 10).'...';

			}else{
				$user_email = $user[0]['email'];
			}

			$txt .= '
		    <tr'.($num?' class="even"':'').'>
		     <td><a href="/?change_user_hash='.$user[0]['hash'].'" title="'.$user[0]['email'].'">'.$user_email.'</a></td>
			 <td><img src="/li/'.$teaser['image'].'" width="70" height="70"></td>
			 <td><a href="'.html_entity_decode($teaser['url']).'" target="_blank">'.$teaser['text'].'</a></td>
			 <td>'.$section_arr[$teaser['section_id']].'</td>
			 <td>'.$view.'</td>
			 <td>'.$click.'</td>
			 <td>'.$ctr.'</td>
			 <td>'.$count_rur.'</td>
			 <td align="right" nowrap>
			';

			if($status == 2){
                $txt .= '
				<a href="./?action=check_teaser_blocked&teaser_id='.$teaser['id'].'&return_page='.urlencode($_SERVER['REQUEST_URI']).'"><img src="/i/delete.gif" width="16" height="16" align="absmiddle" alt="Заблокировать" title="Заблокировать"></a>
				';

			}elseif($status == 4){
                $txt .= '
				<a href="./?action=check_teaser_active&teaser_id='.$teaser['id'].'&return_page='.urlencode($_SERVER['REQUEST_URI']).'"><img src="/i/ok.gif" width="16" height="16" align="absmiddle" alt="Активировать" title="Активировать"></a>
				';
			}

			$txt .= '
			 </td>
			</tr>
			';

			$num = ($num *= -1) + 1;

			$view_cnt += $view;
			$click_cnt += $click;
			$ctr_cnt += $ctr;
			$count_rur_cnt += $count_rur;
		}

	}else{
		$txt .= '
		 <tr>
		  <td colspan="10" align="center">список пуст.</td>
		 </tr>
		';
	}

	$txt .= '
	 <tr>
	  <td colspan="5" class="footer b">Итого:</td>
	  <td class="footer b">'.$view_cnt.'</td>
	  <td class="footer b">'.$click_cnt.'</td>
	  <td class="footer b">'.str_replace(",", ".", @sprintf("%.2f", (100 / $view_cnt) * $click_cnt)).'</td>
	  <td class="footer b">'.str_replace(",", ".", @sprintf("%.2f", $count_rur_cnt)).'</td>
	  <td class="footer"></td>
	 </tr>
	 </tr>
	</table>
	';

	return $txt;
}

function check_teaser_blocked(){
	global $base;
	global $var;

	$teaser_id = get_int_val('teaser_id');

	if($teaser_id){
		$teaser = $base->exec('select id, user_id, url, text
		from '.$var['base_tab_prefix'].'teasers
		where id = "'.$teaser_id.'" and status = "2"');

		if(count($teaser) == 1){
			$base->exec('update '.$var['base_tab_prefix'].'teasers
			set status = "4"
			where id = "'.$teaser_id.'"');

			$user = $base->exec('select id, email from '.$var['base_tab_prefix'].'users
			where id = "'.$teaser[0]['user_id'].'"');

			if(count($user) == 1){
				$mess = 'Размещенный вами тизер заблокирован.'."\r\n\r\n";
				$mess .= 'Текст тизера: '.$teaser[0]['text'].''."\r\n";
				$mess .= 'Ссылка тизера: '.html_entity_decode($teaser[0]['url']).''."\r\n";

				$from = 'no-reply@'.$var['url'];

				$subject = '=?Windows-1251?B?'.trim(@imap_binary('Блокировка тизера')).'?=';
				$headers = 'Return-Path: '.$from.''."\r\n";
				$headers .= 'MIME-Version: 1.0'."\r\n";
				$headers .= 'Content-Transfer-Encoding: 8bit'."\r\n";
				$headers .= 'Content-Type: text/plain; charset="Windows-1251"'."\r\n";
				$headers .= 'X-Mailer: PHP v.'.phpversion()."\r\n";
				$headers .= 'From: =?Windows-1251?B?'.trim(imap_binary(addcslashes($var['title'], "\"!@\\!@(!@)"))).'?= <'.$from.'>'."\r\n";
				$headers .= 'Reply-to: '.$from."\r\n";

				@mail($user[0]['email'], $subject, $mess, $headers);
			}
		}
	}

	@header('location: '.urldecode(get_str_val('return_page')));
	exit;
}

function check_teaser_active(){
	global $base;
	global $var;

	$teaser_id = get_int_val('teaser_id');

	if($teaser_id){
		$teaser = $base->exec('select id, user_id, url, text
		from '.$var['base_tab_prefix'].'teasers
		where id = "'.$teaser_id.'" and status = "4"');

		if(count($teaser) == 1){
			$base->exec('update '.$var['base_tab_prefix'].'teasers
			set status = "2"
			where id = "'.$teaser_id.'"');

			$user = $base->exec('select id, email from '.$var['base_tab_prefix'].'users
			where id = "'.$teaser[0]['user_id'].'"');

			if(count($user) == 1){
				$mess = 'Размещенный вами тизер активирован.'."\r\n\r\n";
				$mess .= 'Текст тизера: '.$teaser[0]['text'].''."\r\n";
				$mess .= 'Ссылка тизера: '.html_entity_decode($teaser[0]['url']).''."\r\n";

				$from = 'no-reply@'.$var['url'];

				$subject = '=?Windows-1251?B?'.trim(@imap_binary('Активация тизера')).'?=';
				$headers = 'Return-Path: '.$from.''."\r\n";
				$headers .= 'MIME-Version: 1.0'."\r\n";
				$headers .= 'Content-Transfer-Encoding: 8bit'."\r\n";
				$headers .= 'Content-Type: text/plain; charset="Windows-1251"'."\r\n";
				$headers .= 'X-Mailer: PHP v.'.phpversion()."\r\n";
				$headers .= 'From: =?Windows-1251?B?'.trim(imap_binary(addcslashes($var['title'], "\"!@\\!@(!@)"))).'?= <'.$from.'>'."\r\n";
				$headers .= 'Reply-to: '.$from."\r\n";

				@mail($user[0]['email'], $subject, $mess, $headers);
			}
		}
	}

	@header('location: '.urldecode(get_str_val('return_page')));
	exit;
}
?>