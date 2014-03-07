<?php
function show_sites(){
	global $base;
	global $var;

	$section = $base->exec('select id, name
	from '.$var['base_tab_prefix'].'sections
	order by binary(name) asc');

	foreach($section as $section){
		$section_arr[$section['id']] = $section['name'];
	}

	$status = array(
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
      <table width=40% align="right">
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

	$txt .= '
		<div class="section_menu">
		<a href="./?action=add"><img src="/i/add.gif" width="16" height="16" align="absmiddle">Добавить сайт</a>
	</div>
	';

	$txt .= '
	<table class="list">
	 <tr class="head">
	  <td>URL</td>
	  <td>Статус</td>
	  <td>Рекламные&nbsp;блоки</td>
	  <td>Показы</td>
	  <td>Клики</td>
	  <td>CTR</td>
	  <td>Доход</td>
	  <td></td>
	 </tr>
	';

	$site = $base->exec('select id, url, sections, status
		from '.$var['base_tab_prefix'].'sites
		where user_id="'.$var['user_id'].'"
		order by dataadd desc');

	$campaignStatistics = campaignsStatExtratc($site, 'sites_stat', 'site_id');

	$view_cnt = 0;
	$click_cnt = 0;

	if(count($site) > 0){
		$num = 0;

		foreach($campaignStatistics as $site){
			$blocks_cnt = $base->exec('select count(*) from '.$var['base_tab_prefix'].'blocks
			where site_id = "'.$site['id'].'"');

			$view = $site['view'];
			$click = $site['click'];
			$ctr = $site['ctr'];
			$count_rur = $site['count_rur'];

			$txt .= '
			 <tr'.($num ? ' class="even"' : '').'>
			  <td nowrap><a href="./?action=edit&site_id='.$site['id'].'">http://'.$site['url'].'/</a></td>
			  <td nowrap>'.$status[$site['status']].'</td>
			  <td><a href="./?action=blocks&site_id='.$site['id'].'">'.$blocks_cnt[0][0].' '.return_word_end($blocks_cnt[0][0], '', 'блок', 'блока', 'блоков').'</a></td>
			  <td>'.$view.'</td>
			  <td>'.$click.'</td>
			  <td>'.$ctr.'</td>
			  <td>'.$count_rur.'</td>
			  <td align="right" nowrap>
			   <a href="./?action=addblock&site_id='.$site['id'].'"><img src="/i/add.gif" width="16" height="16" align="absmiddle" alt="Добавить рекламный блок" title="Добавить рекламный блок"></a>
			';

			if($site['status'] == 0 || $site['status'] == 4){
				$txt .= '
				<img src="/i/play_notactive.gif" width="16" height="16" align="absmiddle" alt="Начать показ рекламы" title="Начать показ рекламы">
				<img src="/i/pause_notactive.gif" width="16" height="16" align="absmiddle" alt="Приостановить показ рекламы" title="Приостановить показ рекламы">
				';

			}elseif($site['status'] == 1){
                $txt .= '
				<img src="/i/play_notactive.gif" width="16" height="16" align="absmiddle" alt="Начать показ рекламы" title="Начать показ рекламы">
				<img src="/i/pause_notactive.gif" width="16" height="16" align="absmiddle" alt="Приостановить показ рекламы" title="Приостановить показ рекламы">
				';

			}elseif($site['status'] == 2){
                $txt .= '
				<img src="/i/play_notactive.gif" width="16" height="16" align="absmiddle" alt="Начать показ рекламы" title="Начать показ рекламы">
				<a href="./?action=change_status&site_id='.$site['id'].'&status=3"><img src="/i/pause_active.gif" width="16" height="16" align="absmiddle" alt="Приостановить показ рекламы" title="Приостановить показ рекламы"></a>
				';

			}elseif($site['status'] == 3){
                $txt .= '
				<a href="./?action=change_status&site_id='.$site['id'].'&status=2"><img src="/i/play_active.gif" width="16" height="16" align="absmiddle" alt="Начать показ рекламы" title="Начать показ рекламы"></a>
				<img src="/i/pause_notactive.gif" width="16" height="16" align="absmiddle" alt="Приостановить показ рекламы" title="Приостановить показ рекламы">
				';
			}

			$txt .= '
			   <a href="./?action=edit&site_id='.$site['id'].'"><img src="/i/edit.gif" width="16" height="16" align="absmiddle" alt="Редактировать информацию о сайте" title="Редактировать информацию о сайте"></a>
			   <a href="./?action=remove&site_id='.$site['id'].'"><img src="/i/delete.gif" width="16" height="16" align="absmiddle" alt="Удалить сайт" title="Удалить сайт" onclick="conf(\'Вы уверены, что хотите удалить сайт?\', this); return false;"></a>
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
		  <td colspan="8" align="center">список пуст.</td>
		 </tr>
		';
	}

	$txt .= '
	 <tr>
	  <td colspan="3" class="footer b">Итого:</td>
	  <td class="footer b">'.$view_cnt.'</td>
	  <td class="footer b">'.$click_cnt.'</td>
	  <td class="footer b">'.str_replace(",", ".", @sprintf("%.2f", (100 / $view_cnt) * $click_cnt)).'</td>
	  <td class="footer b">'.str_replace(",", ".", @sprintf("%.2f", $count_rur_cnt)).'</td>
	  <td class="footer"></td>
	 </tr>
	</table>
	';

	return $txt;
}

function show_site_add(){
	global $base;
	global $var;

	$site_id = get_int_val('site_id');

	if($site_id){
		$site = $base->exec('select url, aliases, ban_teaser, url_coding, sections, stat_logs, status
		from '.$var['base_tab_prefix'].'sites
		where
		user_id="'.$var['user_id'].'"
		and id="'.$site_id.'"');

		if(count($site) != 1){
			@header('location: /webmaster/sites/');
			exit;
		}

		$url = $site[0]['url'];
		$aliases = $site[0]['aliases'];
		$ban_teaser = $site[0]['ban_teaser'];
		$url_coding = $site[0]['url_coding'];
		$sections = $site[0]['sections'];
		$stat_logs = $site[0]['stat_logs'];
	}

	if($_POST['postnewsite'] == 'post'){
		if(!$site_id){
			$url = stripslashes(htmlspecialchars(trim(post_str_val('url'))));
			$url = str_replace('http://', '', $url);
		}

		if(substr($url, 0, 4) == 'www.'){
			$url = substr($url, 4);
		}

		if(substr($url, (strlen($url) - 1)) == '/'){
			$url = substr($url, 0, (strlen($url) - 1));
		}

		$aliases = stripslashes(htmlspecialchars(trim(post_str_val('aliases'))));
		$ban_teaser = stripslashes(htmlspecialchars(trim(post_str_val('ban_teaser'))));
		$url_coding = stripslashes(htmlspecialchars(trim(post_str_val('url_coding'))));
		$sections = $_POST['sections'];
		$stat_logs = stripslashes(htmlspecialchars(trim(post_str_val('stat_logs'))));

		if(!$error && !$site_id){
			if($url == '' || $url == 'http://'){
				$error = 'не указан адрес сайта.';
			}
		}

		if(!$error && !$site_id){
			$site_tmp = $base->exec('select count(*)
			from '.$var['base_tab_prefix'].'sites
			where id != "'.$site_id.'" and url = "'.$url.'"
			or aliases like "%'.$url.'%"');

			if($site_tmp[0][0] != 0){
				$error = 'указан недопустимый адрес сайта. Указанный сайт уже есть в системе.';
			}
		}

		if(!$error){
			if($site_id){
			  $base->exec('update '.$var['base_tab_prefix'].'sites set
				  aliases = "'.$aliases.'",
				  ban_teaser = "'.$ban_teaser.'",
				  url_coding = "'.$url_coding.'",
				  sections = "'.$sections.'",
				  stat_logs = "'.$stat_logs.'"
				  where id="'.$site_id.'"'
			  );

        $postok = 'изменения сохранены.';

		}else{
			$base->exec('
				insert into '.$var['base_tab_prefix'].'sites(
					user_id,
					url,
					aliases,
					ban_teaser,
					url_coding,
					sections,
					stat_logs,
					dataadd,
					last_load,
					price,
					price_cis,
					comment,
					mobil,
					status
				) values (
					"'.$var['user_id'].'",
					"'.$url.'",
					"'.$aliases.'",
					"'.$ban_teaser.'",
					"'.$url_coding.'",
					"'.$sections.'",
					"'.$stat_logs.'",
					"'.$var['datetime'].'",
					"0000-00-00 00:00:00",
					"0.00",
					"0.00",
					"",
					0,
					0
				)');

	      $id = $base->new_id();
				if($id){
					@header('location: /webmaster/sites/');
					exit();
				}else{
					$error = 'не удалось добавить сайт. Попробуйте позже.';
				}
      }
		}
	}

	if($site_id){
		$txt .= '<h1>Редактирование сайта</h1>';

	}else{
		$txt .= '<h1>Добавление сайта</h1>';
	}

	if($error){
		$txt .= '<div class="posterror"><img src="/i/error.gif" width="16" height="16" align="absmiddle"> <b>Ошибка:</b> '.$error.'</div>';
	}

	if($postok){
		$txt .= '<div class="postok"><img src="/i/ok.gif" width="16" height="16" align="absmiddle"> '.$postok.'</div>';
	}

	$txt .= '
	<form action="'.$_SERVER['REQUEST_URI'].'" method="post">
	 <table class="form">
      <tr>
       <td width="25%">URL</td>
       <td><div style="width: 250px;"><input name="url" type="text" value="'.($url ? 'http://'.$url.'/' : 'http://').'" maxlength="255" class="inp"'.($site_id ? ' disabled' : '').'></div></td>
      </tr>
      <tr>
       <td>Алиасы</td>
       <td>
        <div style="width: 350px;"><textarea name="aliases" id="text">'.$aliases.'</textarea></div>
        <div class="dsc">По одному адресу с каждой новой строки.</div>
       </td>
      </tr>
      <tr>
       <td>Исключить показы тизеров, имеющих указанные идентификаторы (teaser_id)</td>
       <td>
        <div style="width: 350px;"><textarea name="ban_teaser" id="text">'.$ban_teaser.'</textarea></div>
        <div class="dsc">По одному через запятую &laquo;,&raquo;</div>
       </td>
      </tr>
      <tr>
       <td>Кодировка сайта</td>
       <td><div style="width: 350px;"><select name="url_coding" class="select">
         <option value="cp1251"'.($url_coding == 'cp1251' ? ' selected' : '').'>Windows-1251</option>
  		 <option value="utf8"'.($url_coding == 'utf8' ? ' selected' : '').'>UTF-8</option>
  		 <option value="koi8r"'.($url_coding == 'koi8r' ? ' selected' : '').'>KOI8-R</option>
		</select></div>
	   </td>
      </tr>
      <tr>
       <td>Укажите тематики,<br/>которые вы разрешаете<br/>к показу на сайте.</td>
       <td>
        <div style="width: 350px;">
        <select name="sections" class="select">
	';

	$section = $base->exec('select id, name
		from '.$var['base_tab_prefix'].'sections
		where status = "1"
		order by binary(name) asc');

	foreach($section as $section){
		
		$txt .= '<option value="'.$section['id'].'"'.($section_arr[$section['id']] == 1 ? ' selected' : '').'>'.$section['name'].' </option>';
	}

  	$txt .='
		</select>
        </div>
       </td>
      </tr>
      <tr>
       <td>Данные для доступа к независимой статистике сайта</td>
       <td>
        <div style="width: 350px;"><textarea name="stat_logs" id="text">'.($stat_logs ? $stat_logs : 'Адрес: '.'
Логин: '.'
Пароль: ').'</textarea></div>
       </td>
      </tr>
      <tr>
       <td></td>
       <td>
        <input name="postnewsite" type="hidden" value="post"><input type="submit" value="Сохранить" class="btn">
        <input type="button" value="Отмена" class="btn" onclick="document.location.href=\'/webmaster/sites/\'">
       </td>
      </tr>
     </table>
    </form>
	';

	return $txt;
}

function show_site_change_status(){
	global $base;
	global $var;

	$status = get_int_val('status');
	$site_id = get_int_val('site_id');

	if($status == 2){
		$site = $base->exec('select count(*)
		from '.$var['base_tab_prefix'].'sites
		where user_id = "'.$var['user_id'].'"
		and id = "'.$site_id.'"
		and status = "3"
		');

		if($site[0][0] == 1){
			$base->exec('update '.$var['base_tab_prefix'].'sites
			set status = "'.$status.'"
			where id="'.$site_id.'"');
		}

	}elseif($status == 3){
		$site = $base->exec('select count(*)
		from '.$var['base_tab_prefix'].'sites
		where user_id = "'.$var['user_id'].'"
		and id = "'.$site_id.'"
		and status = "2"
		');

		if($site[0][0] == 1){
			$base->exec('update '.$var['base_tab_prefix'].'sites
			set status = "'.$status.'"
			where id="'.$site_id.'"');
		}
	}

	@header('location: /webmaster/sites/');
	exit;
}

function show_site_remove(){
	global $base;
	global $var;

	$site_id = get_int_val('site_id');

	$site = $base->exec('select url
	from '.$var['base_tab_prefix'].'sites
	where user_id = "'.$var['user_id'].'"
	and id = "'.$site_id.'"
	');

	if(count($site) == 1){
		$base->exec('delete from '.$var['base_tab_prefix'].'sites
		where user_id = "'.$var['user_id'].'"
		and id = "'.$site_id.'"');

		$base->exec('delete from '.$var['base_tab_prefix'].'blocks
		where user_id = "'.$var['user_id'].'"
		and site_id = "'.$site_id.'"');
	}

	@header('location: /webmaster/sites/');
	exit;
}
?>