<?php
function show_administrator_sites(){
	global $base;
	global $var;

	$status = get_str_val('status');

	if($_GET['status'] == ''){
		@header('location: ./?action=sites&status=1');
		exit;
	}

	$section = $base->exec('select id, name
	from '.$var['base_tab_prefix'].'sections
	order by status asc');

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

	$site_new_cnt = $base->exec('select count(*) from '.$var['base_tab_prefix'].'sites
	where status = "0"');

	$site_mod_cnt = $base->exec('select count(*) from '.$var['base_tab_prefix'].'sites
	where status = "1"');

	$site_active_cnt = $base->exec('select count(*) from '.$var['base_tab_prefix'].'sites
	where status = "2"');

	$site_stoped_cnt = $base->exec('select count(*) from '.$var['base_tab_prefix'].'sites
	where status = "3"');

	$site_blocked_cnt = $base->exec('select count(*) from '.$var['base_tab_prefix'].'sites
	where status = "4"');

	$txt .= showMenu('', '',
		array(
		'Новые[('.$site_new_cnt[0][0].')]' => '/administrator/?action=sites&status=0',
		'На модерации[('.$site_mod_cnt[0][0].')]' => '/administrator/?action=sites&status=1',
		'Активные[('.$site_active_cnt[0][0].')]' => '/administrator/?action=sites&status=2',
		'Приостановленные[('.$site_stoped_cnt[0][0].')]' => '/administrator/?action=sites&status=3',
		'Заблокированные[('.$site_blocked_cnt[0][0].')]' => '/administrator/?action=sites&status=4',
		), 'navigation_sub_menu', 'td', false
	);

	$txt .= '
	<table class="list">
	 <tr class="head">
	  <td>#id</td>
	';

	if($status == '0' || $status == '1'){
		$txt .= '
		<td>URL</td><td>Пользователь</td><td>Доступ к статистике</td><td></td>
		';

	}elseif($status == '2'){
		$txt .= '
		<td>URL</td><td>Пользователь</td><td>Доступ к статистике</td><td>Показы</td><td>Клики</td><td>CTR</td><td>Доход</td><td></td>
		';

	}elseif($status == '3'){
		$txt .= '
		<td>URL</td><td>Пользователь</td><td>Доступ к статистике</td><td>Показы</td><td>Клики</td><td>CTR</td><td>Доход</td>
		';

	}elseif($status == '4'){
		$txt .= '
		<td>URL</td><td>Пользователь</td><td>Доступ к статистике</td><td>Причина</td><td>Показы</td><td>Клики</td><td>CTR</td><td>Доход</td><td></td>
		';
	}

	$txt .= '
	</tr>
	';

	$site = $base->exec('select id, user_id, url, stat_logs, comment, status
	from '.$var['base_tab_prefix'].'sites
	where status = "'.$status.'"
	order by binary(url) asc');

    $campaignStatistics = campaignsStatExtratc($site, 'sites_stat', 'site_id');

    $view_cnt = 0;
	$click_cnt = 0;

	if(count($campaignStatistics) > 0){
		$num = 0;

		foreach($campaignStatistics as $site){
			if(strlen($site['url']) > 12){
				$site_url = substr($site['url'], 0, 12).'...';

			}else{
				$site_url = $site['url'];
			}

			$user = $base->exec('select id, email, hash
			from '.$var['base_tab_prefix'].'users
			where id = "'.$site['user_id'].'"');

			if(strlen($user[0]['email']) > 10){
				$user_email = substr($user[0]['email'], 0, 10).'...';

			}else{
				$user_email = $user[0]['email'];
			}

			if($status == '0' || $status == '1'){
				$site['stat_logs'] = str_replace("\n", "<br/>", $site['stat_logs']);

				$txt .= '
				 <tr'.($num ? ' class="even"' : '').'>
				  <td>'.$site['id'].'</td>
				  <td nowrap><a href="http://'.$site['url'].'/" title="http://'.$site['url'].'/" target="_blank">http://'.$site_url.'/</a></td>
				  <td><a href="/?change_user_hash='.$user[0]['hash'].'" title="'.$user[0]['email'].'">'.$user_email.'</a></td>
				  <td>'.$site['stat_logs'].'</td>
				  <td align="right" nowrap>
				   <a href="./?action=check_mod_active&site_id='.$site['id'].'&return_page='.urlencode($_SERVER['REQUEST_URI']).'"><img src="/i/ok.gif" width="16" height="16" align="absmiddle" alt="Принять в сеть" title="Принять в сеть"></a>
				   <a href="./?action=check_mod_blocked&site_id='.$site['id'].'&return_page='.urlencode($_SERVER['REQUEST_URI']).'"><img src="/i/delete.gif" width="16" height="16" align="absmiddle" alt="Отклонить" title="Отклонить"></a>
				  </td>
				 </tr>
				';

			}elseif($status == '2'){
              $view = $site['view'];
              $click = $site['click'];
              $ctr = $site['ctr'];
              $count_rur = $site['count_rur'];
			  $site['stat_logs'] = str_replace("\n", "<br/>", $site['stat_logs']);

			  $txt .= '
				 <tr'.($num ? ' class="even"' : '').'>
				  <td>'.$site['id'].'</td>
				  <td nowrap><a href="http://'.$site['url'].'/" title="http://'.$site['url'].'/" target="_blank">http://'.$site_url.'/</a></td>
				  <td><a href="/?change_user_hash='.$user[0]['hash'].'" title="'.$user[0]['email'].'">'.$user_email.'</a></td>
				  <td>'.$site['stat_logs'].'</td>
				  <td>'.$view.'</td>
			  	  <td>'.$click.'</td>
			  	  <td>'.$ctr.'</td>
			  	  <td>'.$count_rur.'</td>
				  <td align="right" nowrap>
				   <a href="./?action=editsite&site_id='.$site['id'].'"><img src="/i/edit.gif" width="16" height="16" align="absmiddle" alt="Редактировать" title="Редактировать"></a>
				   <a href="./?action=check_blocked&site_id='.$site['id'].'&return_page='.urlencode($_SERVER['REQUEST_URI']).'"><img src="/i/delete.gif" width="16" height="16" align="absmiddle" alt="Заблокировать" title="Заблокировать"></a>
				  </td>
				 </tr>
				';

				$view_cnt += $view;
				$click_cnt += $click;
				$ctr_cnt += $ctr;
				$count_rur_cnt += $count_rur;

			}elseif($status == '3'){
				$stat = $base->exec('select sum(view) as view, sum(click) as click, sum(count_rur) as count_rur
				from '.$var['base_tab_prefix'].'sites_stat
				where site_id = "'.$site['id'].'"');

				$view = ($stat[0]['view'] ? $stat[0]['view'] : 0);
				$click = ($stat[0]['click'] ? $stat[0]['click'] : 0);
				$ctr = str_replace(",", ".", @sprintf("%.2f", (100 / $view) * $click));
				$count_rur = ($stat[0]['count_rur'] ? $stat[0]['count_rur'] : 0.00);

				$site['stat_logs'] = str_replace("\n", "<br/>", $site['stat_logs']);

				$txt .= '
				 <tr'.($num ? ' class="even"' : '').'>
				  <td>'.$site['id'].'</td>
				  <td nowrap><a href="http://'.$site['url'].'/" title="http://'.$site['url'].'/" target="_blank">http://'.$site_url.'/</a></td>
				  <td><a href="/?change_user_hash='.$user[0]['hash'].'" title="'.$user[0]['email'].'">'.$user_email.'</a></td>
				  <td>'.$site['stat_logs'].'</td>
				  <td>'.$view.'</td>
			  	  <td>'.$click.'</td>
			  	  <td>'.$ctr.'</td>
			  	  <td>'.$count_rur.'</td>
				 </tr>
				';

				$view_cnt += $view;
				$click_cnt += $click;
				$ctr_cnt += $ctr;
				$count_rur_cnt += $count_rur;

			}elseif($status == '4'){
				$stat = $base->exec('select sum(view) as view, sum(click) as click, sum(count_rur) as count_rur
				from '.$var['base_tab_prefix'].'sites_stat
				where site_id = "'.$site['id'].'"');

				$view = ($stat[0]['view'] ? $stat[0]['view'] : 0);
				$click = ($stat[0]['click'] ? $stat[0]['click'] : 0);
				$ctr = str_replace(",", ".", @sprintf("%.2f", (100 / $view) * $click));
				$count_rur = ($stat[0]['count_rur'] ? $stat[0]['count_rur'] : 0.00);

				$site['stat_logs'] = str_replace("\n", "<br/>", $site['stat_logs']);

				$txt .= '
				 <tr'.($num ? ' class="even"' : '').'>
				  <td>'.$site['id'].'</td>
				  <td nowrap><a href="http://'.$site['url'].'/" title="http://'.$site['url'].'/" target="_blank">http://'.$site_url.'/</a></td>
				  <td><a href="/?change_user_hash='.$user[0]['hash'].'" title="'.$user[0]['email'].'">'.$user_email.'</a></td>
				  <td>'.$site['stat_logs'].'</td>
				  <td>'.$site['comment'].'</td>
				  <td>'.$view.'</td>
			  	  <td>'.$click.'</td>
			  	  <td>'.$ctr.'</td>
			  	  <td>'.$count_rur.'</td>
			  	  <td align="right" nowrap>
				   <a href="./?action=check_active&site_id='.$site['id'].'&return_page='.urlencode($_SERVER['REQUEST_URI']).'"><img src="/i/ok.gif" width="16" height="16" align="absmiddle" alt="Активировать" title="Активировать"></a>
				  </td>
				 </tr>
				';

				$view_cnt += $view;
				$click_cnt += $click;
				$ctr_cnt += $ctr;
				$count_rur_cnt += $count_rur;
			}
			$num = ($num *= -1) + 1;
		}

	}else{
		$txt .= '
		 <tr>
		  <td colspan="9" align="center">список пуст.</td>
		 </tr>
		';
	}

	if($status == '0' || $status == '1'){
		$txt .= '
	 	<tr><td  class="footer b" colspan="5"></tr>
	 	';

	}elseif($status == '2'){
		$txt .= '
		 <tr>
		  <td colspan="4" class="footer b">Итого:</td>
		  <td class="footer b">'.$view_cnt.'</td>
		  <td class="footer b">'.$click_cnt.'</td>
		  <td class="footer b">'.str_replace(",", ".", @sprintf("%.2f", (100 / $view_cnt) * $click_cnt)).'</td>
		  <td class="footer b">'.str_replace(",", ".", @sprintf("%.2f", $count_rur_cnt)).'</td>
		  <td class="footer"></td>
		 </tr>
		';

	}elseif($status == '3'){
		$txt .= '
		 <tr>
		  <td colspan="4" class="footer b">Итого:</td>
		  <td class="footer b">'.$view_cnt.'</td>
		  <td class="footer b">'.$click_cnt.'</td>
		  <td class="footer b">'.str_replace(",", ".", @sprintf("%.2f", (100 / $view_cnt) * $click_cnt)).'</td>
		  <td class="footer b">'.str_replace(",", ".", @sprintf("%.2f", $count_rur_cnt)).'</td>
		 </tr>
		';

	}elseif($status == '4'){
		$txt .= '
		 <tr>
		  <td colspan="5" class="footer b">Итого:</td>
		  <td class="footer b">'.$view_cnt.'</td>
		  <td class="footer b">'.$click_cnt.'</td>
		  <td class="footer b">'.str_replace(",", ".", @sprintf("%.2f", (100 / $view_cnt) * $click_cnt)).'</td>
		  <td class="footer b">'.str_replace(",", ".", @sprintf("%.2f", $count_rur_cnt)).'</td>
		  <td class="footer"></td>
		 </tr>
		';
	}

	$txt .= '
	</table>
	';

	return $txt;
}

function show_administrator_site_edit(){
	global $base;
	global $var;

	$site_id = get_int_val('site_id');

	if($site_id){
	  $site = $base->exec('select id, url, price, price_cis, sections
	  from '.$var['base_tab_prefix'].'sites
	  where id = "'.$site_id.'"');

	  $price = $site[0]['price'];
	  $price_cis = $site[0]['price_cis'];
	}

	if(count($site) == 1){
	  if($_POST['postsiteedit'] == 'post'){
		$price = post_float_val('price');
		$price_cis = post_float_val('price_cis');

		$base->exec('update '.$var['base_tab_prefix'].'sites
		  set price = "'.$price.'", price_cis = "'.$price_cis.'"
		  where id = "'.$site_id.'"');

        $postok = 'Изменения сохранены.';
	  }
	  $txt .= '<h1>Редактирование сайта</h1>';

	  if($postok){
		$txt .= '<div class="postok"><img src="/i/ok.gif" width="16" height="16" align="absmiddle"> '.$postok.'</div>';
	  }
	  if($error){
		$txt .= '<div class="posterror"><img src="/i/error.gif" width="16" height="16" align="absmiddle"> <b>Ошибка:</b> '.$error.'</div>';
	  }

    $txt .='
      <form action="'.$_SERVER['REQUEST_URI'].'" method="post">
       <table class="form">
        <tr>
         <td width="30%">URL: </td>
         <td>http://'.$site[0]['url'].'/</td>
        </tr>
        
        <tr>
         <td>Вознаграждение за клик (Россия)</td>
         <td><input name="price" type="text" value="'.($price ? $price : '0.00').'" class="inp"></td>
        </tr>
        
        <tr>
         <td>Вознаграждение за клик (СНГ)</td>
         <td><input name="price_cis" type="text" value="'.($price_cis ? $price_cis : '0.00').'" class="inp"></td>
        </tr>
  
        <tr>
         <td></td>
         <td><input name="postsiteedit" type="hidden" value="post"><input type="submit" value="Сохранить" class="btn"></td>
        </tr>
       </table>
      </form>
      ';
	}else{
		@header('location: '.$_SERVER['HTTP_REFERER']);
		exit;
	}

	return $txt;
}

function check_mod_active(){
	global $base;
	global $var;

	$site_id = get_int_val('site_id');

	if($site_id){
		$site = $base->exec('select id, user_id, url
		from '.$var['base_tab_prefix'].'sites
		where id = "'.$site_id.'" and status = "0"
		or id = "'.$site_id.'" and status = "1"');

		if(count($site) == 1){
			$base->exec('update '.$var['base_tab_prefix'].'sites
			set status = "2"
			where id = "'.$site_id.'"');

			$user = $base->exec('select id, email from '.$var['base_tab_prefix'].'users
			where id = "'.$site[0]['user_id'].'"');

			if(count($user) == 1){
            	$mess = 'Ваш сайт http://'.$site[0]['url'].'/ принят в сеть.';

           		$from = 'no-reply@'.$var['url'];

           		$subject = '=?Windows-1251?B?'.trim(@imap_binary('Модерация: сайт принят')).'?=';
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

function check_active(){
	global $base;
	global $var;

	$site_id = get_int_val('site_id');

	if($site_id){
		$site = $base->exec('select id, user_id, url
		from '.$var['base_tab_prefix'].'sites
		where id = "'.$site_id.'" and status = "4"');

		if(count($site) == 1){
			$base->exec('update '.$var['base_tab_prefix'].'sites
			set status = "2"
			where id = "'.$site_id.'"');

			$user = $base->exec('select id, email from '.$var['base_tab_prefix'].'users
			where id = "'.$site[0]['user_id'].'"');

			if(count($user) == 1){
            	$mess = 'Ваш сайт http://'.$site[0]['url'].'/ активирован.';

           		$from = 'no-reply@'.$var['url'];

           		$subject = '=?Windows-1251?B?'.trim(@imap_binary('Активация сайта')).'?=';
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

function check_mod_blocked(){
	global $base;
	global $var;

	$site_id = get_int_val('site_id');

	if($site_id){
		$site = $base->exec('select id, user_id, url
		from '.$var['base_tab_prefix'].'sites
		where id = "'.$site_id.'" and status = "0"
		or id = "'.$site_id.'" and status = "1"');

	}

	if(count($site) == 1){
		if($_POST['postmodblocked'] == 'post'){
			$text = post_str_val('text');

			$base->exec('update '.$var['base_tab_prefix'].'sites
			set status = "4", comment = "'.$text.'"
			where id = "'.$site_id.'"');

			$user = $base->exec('select id, email from '.$var['base_tab_prefix'].'users
			where id = "'.$site[0]['user_id'].'"');

			if(count($user) == 1){
            	$mess = 'Ваш сайт http://'.$site[0]['url'].'/ НЕ ПРИНЯТ в сеть.';

            	if($text != ''){
            		$mess .= "\r\n\r\nПричина: ".$text;
            	}

           		$from = 'no-reply@'.$var['url'];

           		$subject = '=?Windows-1251?B?'.trim(@imap_binary('Модерация: сайт НЕ ПРИНЯТ')).'?=';
				$headers = 'Return-Path: '.$from.''."\r\n";
				$headers .= 'MIME-Version: 1.0'."\r\n";
				$headers .= 'Content-Transfer-Encoding: 8bit'."\r\n";
				$headers .= 'Content-Type: text/plain; charset="Windows-1251"'."\r\n";
				$headers .= 'X-Mailer: PHP v.'.phpversion()."\r\n";
				$headers .= 'From: =?Windows-1251?B?'.trim(imap_binary(addcslashes($var['title'], "\"!@\\!@(!@)"))).'?= <'.$from.'>'."\r\n";
				$headers .= 'Reply-to: '.$from."\r\n";

				@mail($user[0]['email'], $subject, $mess, $headers);
			}

			@header('location: '.urldecode(get_str_val('return_page')));
			exit;
		}

		$txt .= '<h1>Отказ в допуске сайта в сеть</h1>';

		$txt .='
		<form action="'.$_SERVER['REQUEST_URI'].'" method="post">
		 <table class="form">
	      <tr>
	       <td width="30%">URL: </td>
	       <td>http://'.$site[0]['url'].'/</td>
	      </tr>
	      <tr>
	       <td>Причина отказа</td>
	       <td><textarea name="text">'.$text.'</textarea></td>
	      </tr>
	      <tr>
	       <td></td>
	       <td><input name="postmodblocked" type="hidden" value="post"><input type="submit" value="Сохранить" class="btn"></td>
	      </tr>
		 </table>
		</form>
		';
	}

	return $txt;
}

function check_blocked(){
	global $base;
	global $var;

	$site_id = get_int_val('site_id');

	if($site_id){
		$site = $base->exec('select id, user_id, url
		from '.$var['base_tab_prefix'].'sites
		where id = "'.$site_id.'" and status = "2"');

	}

	if(count($site) == 1){
		if($_POST['postblocked'] == 'post'){
			$text = post_str_val('text');

			$base->exec('update '.$var['base_tab_prefix'].'sites
			set status = "4", comment = "'.$text.'"
			where id = "'.$site_id.'"');

			$user = $base->exec('select id, email from '.$var['base_tab_prefix'].'users
			where id = "'.$site[0]['user_id'].'"');

			if(count($user) == 1){
            	$mess = 'Ваш сайт http://'.$site[0]['url'].'/ заблокирован.';

            	if($text != ''){
            		$mess .= "\r\n\r\nПричина: ".$text;
            	}

           		$from = 'no-reply@'.$var['url'];

           		$subject = '=?Windows-1251?B?'.trim(@imap_binary('Блокировка сайта')).'?=';
				$headers = 'Return-Path: '.$from.''."\r\n";
				$headers .= 'MIME-Version: 1.0'."\r\n";
				$headers .= 'Content-Transfer-Encoding: 8bit'."\r\n";
				$headers .= 'Content-Type: text/plain; charset="Windows-1251"'."\r\n";
				$headers .= 'X-Mailer: PHP v.'.phpversion()."\r\n";
				$headers .= 'From: =?Windows-1251?B?'.trim(imap_binary(addcslashes($var['title'], "\"!@\\!@(!@)"))).'?= <'.$from.'>'."\r\n";
				$headers .= 'Reply-to: '.$from."\r\n";

				@mail($user[0]['email'], $subject, $mess, $headers);
			}

			@header('location: '.urldecode(get_str_val('return_page')));
			exit;
		}

		$txt .= '<h1>Блокировка сайта</h1>';

		$txt .='
		<form action="'.$_SERVER['REQUEST_URI'].'" method="post">
		 <table class="form">
	      <tr>
	       <td width="30%">URL: </td>
	       <td>http://'.$site[0]['url'].'/</td>
	      </tr>
	      <tr>
	       <td>Причина блокировки</td>
	       <td><textarea name="text">'.$text.'</textarea></td>
	      </tr>
	      <tr>
	       <td></td>
	       <td><input name="postblocked" type="hidden" value="post"><input type="submit" value="Сохранить" class="btn"></td>
	      </tr>
		 </table>
		</form>
		';
	}

	return $txt;
}
?>