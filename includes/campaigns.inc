<?php
function show_campaigns(){
	global $base;
	global $var;

	$status = array(
		0 => 'новая',
		1 => 'на модерации',
		2 => 'идут показы',
		3 => 'показы<br/>приостановлены',
		4 => 'заблокирована'
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
    <div class="section_menu margin_top_100">
      <a href="./?action=add"><img src="/i/add.gif" width="16" height="16" align="absmiddle">Создать кампанию</a>
    </div>
	';

	$txt .= '
	<table class="list">
	 <tr class="head">
	  <td>#id</td>
	  <td>Название</td>
	  <td>Статус</td>
	  <td>Объявления</td>
	  <td>Показы</td>
	  <td>Клики</td>
	  <td>CTR</td>
	  <td>Расход</td>
	  <td></td>
	 </tr>
	';

	$campaign = $base->exec('select id, name, status
    from '.$var['base_tab_prefix'].'campaigns
    where user_id="'.$var['user_id'].'"
    order by dataadd desc');
  
  $campaignStatistics = campaignsStatExtratc($campaign, 'campaigns_stat', 'campaign_id');
  
  $view_cnt = 0;
	$click_cnt = 0;

	if(count($campaignStatistics) > 0){
		$num = 0;

		foreach($campaignStatistics as $campaign){
      $teasers_cnt = $base->exec('select count(*) from '.$var['base_tab_prefix'].'teasers
        where campaign_id = "'.$campaign['id'].'"');
      
      $view = $campaign['view'];
      
      $click = $campaign['click'];
      
      $ctr = $campaign['ctr'];
      
      $count_rur = $campaign['count_rur'];

			$txt .= '
			<tr'.($num?' class="even"':'').'>
			 <td>'.$campaign['id'].'</td>
			 <td><a href="./?action=edit&campaign_id='.$campaign['id'].'">'.$campaign['name'].'</a></td>
			 <td>'.$status[$campaign['status']].'</td>
			 <td nowrap><a href="./?action=teasers&campaign_id='.$campaign['id'].'">'.$teasers_cnt[0][0].' '.return_word_end($teasers_cnt[0][0], 'объявлен', 'ие', 'ия', 'ий').'</a></td>
	  	 <td>'.$view.'</td>
			 <td>'.$click.'</td>
			 <td>'.$ctr.'</td>
			 <td>'.$count_rur.'</td>
			 <td align="right" nowrap>
			 <a href="./?action=addteaser&campaign_id='.$campaign['id'].'"><img src="/i/add.gif" width="16" height="16" align="absmiddle" alt="Добавить объявление в кампанию" title="Добавить объявление в кампанию"></a>';

			if($campaign['status'] == 2){
        $txt .= '<img src="/i/play_notactive.gif" width="16" height="16" align="absmiddle" alt="Начать показ рекламы" title="Начать показ рекламы">
          <a href="./?action=change_status&campaign_id='.$campaign['id'].'&status=3"><img src="/i/pause_active.gif" width="16" height="16" align="absmiddle" alt="Приостановить показ рекламы" title="Приостановить показ рекламы"></a>';

			}elseif($campaign['status'] == 3){
        $txt .= '<a href="./?action=change_status&campaign_id='.$campaign['id'].'&status=2"><img src="/i/play_active.gif" width="16" height="16" align="absmiddle" alt="Начать показ рекламы" title="Начать показ рекламы"></a>
          <img src="/i/pause_notactive.gif" width="16" height="16" align="absmiddle" alt="Приостановить показ рекламы" title="Приостановить показ рекламы">';
        }

        $txt .= '
          <a href="./?action=edit&campaign_id='.$campaign['id'].'"><img src="/i/edit.gif" width="16" height="16" align="absmiddle" alt="Редактировать кампанию" title="Редактировать кампанию"></a>
          <a href="./?action=remove&campaign_id='.$campaign['id'].'"><img src="/i/delete.gif" width="16" height="16" align="absmiddle" alt="Удалить кампанию" title="Удалить кампанию" onclick="conf(\'Вы уверены, что хотите удалить кампанию?\', this); return false;"></a>
         </td>
        </tr>';

			$num = ($num *= -1) + 1;

			$view_cnt += $view;
			$click_cnt += $click;
			$ctr_cnt += $ctr;
			$count_rur_cnt += $count_rur;
		}

	}else{
		$txt .= '
		 <tr>
		  <td colspan="9" align="center">список пуст.</td>
		 </tr>
		';
	}

	$txt .= '
	 <tr>
	  <td colspan="4" class="footer b">Итого:</td>
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

function show_campaign_add(){
	global $base;
	global $var;

	$campaign_id = get_int_val('campaign_id');

	if($campaign_id){
		$campaign = $base->exec('select name, section_id, ban_site, ban_region, ban_country,
		ban_hour, ban_week_day, labels, subid
		from '.$var['base_tab_prefix'].'campaigns
		where user_id = "'.$var['user_id'].'"
		and id = "'.$campaign_id.'"');

		if(count($campaign) != 1){
			@header('location: /advertiser/campaigns/');
			exit;
		}

		$name = $campaign[0]['name'];
		$section_id = $campaign[0]['section_id'];
		$ban_site = $campaign[0]['ban_site'];
    $labels = $campaign[0]['labels'];
    $subid = $campaign[0]['subid'];

		$ban_regions = explode("~", $campaign[0]['ban_region']);
        foreach($ban_regions as $ban_region){
			if($ban_region == '')continue;
			$ban_region_arr[$ban_region] = 1;
		}

		$ban_countrys = explode("~", $campaign[0]['ban_country']);
        foreach($ban_countrys as $ban_country){
			if($ban_country == '')continue;
			$ban_country_arr[$ban_country] = 1;
		}

		$ban_hour = explode("~", $campaign[0]['ban_hour']);
        foreach($ban_hour as $ban_hour){
			if($ban_hour == '')continue;
			$ban_hour_arr[$ban_hour] = 1;
		}

		$ban_week_day = explode("~", $campaign[0]['ban_week_day']);
        foreach($ban_week_day as $ban_week_day){
			if($ban_week_day == '')continue;
			$ban_week_day_arr[$ban_week_day] = 1;
		}
	}

	if($_POST['postnewcampaign'] == 'post'){
    $section_id = post_int_val('section_id');
    $name = stripslashes(htmlspecialchars(trim(post_str_val('name'))));

		$ban_site = stripslashes(htmlspecialchars(trim(post_str_val('ban_site'))));
		$ban_regions = $_POST['ban_regions'];
		$ban_region_arr = '';

		if(count($ban_regions) > 0){
			foreach($ban_regions as $ban_region){
				if($ban_region == '')continue;
				$ban_region_arr[$ban_region] = 1;
				$ban_region_list .= '~'.$ban_region.'~';
			}
		}

		$ban_countrys = $_POST['ban_countrys'];
		$ban_country_arr = '';
		if(count($ban_countrys) > 0){
			foreach($ban_countrys as $ban_country){
				if($ban_country == '')continue;
				$ban_country_arr[$ban_country] = 1;
				$ban_country_list .= '~'.$ban_country.'~';
			}
		}

		$ban_hour = $_POST['ban_hour'];
		$ban_hour_arr = '';
		if(count($ban_hour) > 0){
			foreach($ban_hour as $ban_hour){
				if($ban_hour == '')continue;
				$ban_hour_arr[$ban_hour] = 1;
				$ban_hour_list .= '~'.$ban_hour.'~';
			}
		}

		$ban_week_day = $_POST['ban_week_day'];
		$ban_week_day_arr = '';
		if(count($ban_week_day) > 0){
			foreach($ban_week_day as $ban_week_day){
				if($ban_week_day == '')continue;
				$ban_week_day_arr[$ban_week_day] = 1;
				$ban_week_day_list .= '~'.$ban_week_day.'~';
			}
		}

		$labels = stripslashes(htmlspecialchars(trim(post_str_val('labels'))));
    $subid = trim(post_str_val('subid_mark'));

		if(!$name){
			$error = 'не указано название.';
		}

		if(!$error){
			if(!$section_id){
				$error = 'не указана тематика.';
			}
		}

		if(!$error){
			if($campaign_id){
        $base->exec('update '.$var['base_tab_prefix'].'campaigns
          set
          section_id="'.$section_id.'",
          name="'.$name.'",
          ban_site="'.$ban_site.'",
          ban_region="'.$ban_region_list.'",
          ban_country="'.$ban_country_list.'",
          ban_hour="'.$ban_hour_list.'",
          ban_week_day="'.$ban_week_day_list.'",
          labels = "'.$labels.'",
          subid = "'.$subid.'"
          where id="'.$campaign_id.'"');

				$base->exec('update '.$var['base_tab_prefix'].'teasers
          set
          ban_site="'.$ban_site.'",
          ban_region="'.$ban_region_list.'",
          ban_country="'.$ban_country_list.'",
          ban_hour="'.$ban_hour_list.'",
          ban_week_day="'.$ban_week_day_list.'"
          where campaign_id="'.$campaign_id.'"');

				$postok = 'изменения сохранены.';

			}else{
				$base->exec('
	        	insert into '.$var['base_tab_prefix'].'campaigns(
           user_id,
           section_id,
	         name,
	         ban_site,
	         ban_region,
	         ban_country,
	         ban_hour,
	         ban_week_day,
	        	dataadd,
	        	labels,
           subid,
	        	status
	        	)values(
	         "'.$var['user_id'].'",
	         "'.$section_id.'",
	         "'.$name.'",
	         "'.$ban_site.'",
	         "'.$ban_region_list.'",
	         "'.$ban_country_list.'",
	         "'.$ban_hour_list.'",
	         "'.$ban_week_day_list.'",
	         "'.$var['datetime'].'",
	         "'.$labels.'",
           "'.$subid.'",
	         "2"
	        	)
	     ');

	      $id = $base->new_id();
				if($id){
					@header('location: /advertiser/campaigns/');
					exit;
				}else{
					$error = 'не удалось создать кампанию. Попробуйте позже.';
				}
			}
		}
	}

	if($campaign_id){
		$txt .= '<h1>Редактирование кампании</h1>';
	}else{
		$txt .= '<h1>Создание кампании</h1>';
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
       <td width="25%">Название</td>
       <td><div style="width: 250px;"><input name="name" type="text" value="'.$name.'" maxlength="255" class="inp"></div></td>
      </tr>
      <tr>
       <td>Тематика</td>
       <td>
        <div style="width: 350px;">
        <select name="section_id" class="select">
	';

	$sections = $base->exec('select id, name
    from '.$var['base_tab_prefix'].'sections
    where status = "1"
    order by binary(name) asc');

	foreach($sections as $sections){
		$txt .= '<option value="'.$sections['id'].'"'.($sections['id'] == $section_id ? ' selected' : '').'>'.$sections['name'].' </option>';
	}

  	$txt .='
		</select>
        </div>
       </td>
      </tr>
      <tr>
       <td>Цена за клик</td>
       <td>
        <div class="dsc">
         Настройки по цене за клик будут использоваться<br/>
         по умолчанию (в разделе тематики) для всех объявлений кампании.<br/>
         Однако для каждого конкретного объявления (в разделе Редактирования сайта)<br/>
         можно будет указать индивидуальные настройки.
        </div>
       </td>
      </tr>
      <tr>
       <td>Исключить показы<br/>на сайтах</td>
       <td>
        <div style="width: 350px;"><textarea name="ban_site" id="text">'.$ban_site.'</textarea></div>
        <div class="dsc">По одному адресу с каждой новой строки.</div>
       </td>
      </tr>
      <tr>
       <td>Исключить показы<br/>жителям выбранных<br/>стран</td>
       <td>
        <div style="width: 350px;">
        <select name="ban_countrys[]" class="select" size="8" multiple>
        <option value="Прочие страны"'.($ban_country_arr['Прочие страны'] == 1 ? ' selected' : '').'>Прочие страны </option>
	';

	$country = $base->exec('select country, country_name
	from '.$var['base_tab_prefix'].'ipgeobase
	group by country
	order by binary(country_name) asc');

	foreach($country as $country){
		$txt .= '<option value="'.$country['country'].'"'.($ban_country_arr[$country['country']] == 1 ? ' selected' : '').'>'.$country['country_name'].' </option>';
	}

  	$txt .='
        </select>
        </div>
        <div class="dsc">Вы можете указать несколько стран, удерживая клавишу Ctrl</div>
       </td>
      </tr>
      <tr>
       <td>Исключить показы<br/>жителям выбранных<br/>регионов России</td>
       <td>
        <div style="width: 350px;">
        <select name="ban_regions[]" class="select" size="8" multiple>
        <option value="Прочие регионы"'.($ban_region_arr['Прочие регионы'] == 1 ? ' selected' : '').'>Прочие регионы </option>
	';

	$region = $base->exec('select region
	from '.$var['base_tab_prefix'].'ipgeobase
	where country = "RU"
	group by region
	order by binary(region) asc');

	foreach($region as $region){
		$txt .= '<option value="'.$region['region'].'"'.($ban_region_arr[$region['region']] == 1 ? ' selected' : '').'>'.$region['region'].' </option>';
	}

  	$txt .='
        </select>
        </div>
        <div class="dsc">Вы можете указать несколько регионов, удерживая клавишу Ctrl</div>
       </td>
      </tr>
      <tr>
       <td>Исключить показы<br/>в указанные часы</td>
       <td>

        <table>
         <tr>
          <td><input name="ban_hour[]" id="ban_hour_0" type="checkbox" value="0"'.($ban_hour_arr[0] == 1 ? ' checked' : '').'><label for="ban_hour_0">0</label></td>
          <td><input name="ban_hour[]" id="ban_hour_1" type="checkbox" value="1"'.($ban_hour_arr[1] == 1 ? ' checked' : '').'><label for="ban_hour_1">1</label></td>
          <td><input name="ban_hour[]" id="ban_hour_2" type="checkbox" value="2"'.($ban_hour_arr[2] == 1 ? ' checked' : '').'><label for="ban_hour_2">2</label></td>
          <td><input name="ban_hour[]" id="ban_hour_3" type="checkbox" value="3"'.($ban_hour_arr[3] == 1 ? ' checked' : '').'><label for="ban_hour_3">3</label></td>
          <td><input name="ban_hour[]" id="ban_hour_4" type="checkbox" value="4"'.($ban_hour_arr[4] == 1 ? ' checked' : '').'><label for="ban_hour_4">4</label></td>
          <td><input name="ban_hour[]" id="ban_hour_5" type="checkbox" value="5"'.($ban_hour_arr[5] == 1 ? ' checked' : '').'><label for="ban_hour_5">5</label></td>
          <td><input name="ban_hour[]" id="ban_hour_6" type="checkbox" value="6"'.($ban_hour_arr[6] == 1 ? ' checked' : '').'><label for="ban_hour_6">6</label></td>
          <td><input name="ban_hour[]" id="ban_hour_7" type="checkbox" value="7"'.($ban_hour_arr[7] == 1 ? ' checked' : '').'><label for="ban_hour_7">7</label></td>
         </tr>
         <tr>
          <td><input name="ban_hour[]" id="ban_hour_8" type="checkbox" value="8"'.($ban_hour_arr[8] == 1 ? ' checked' : '').'><label for="ban_hour_8">8</label></td>
          <td><input name="ban_hour[]" id="ban_hour_9" type="checkbox" value="9"'.($ban_hour_arr[9] == 1 ? ' checked' : '').'><label for="ban_hour_9">9</label></td>
          <td><input name="ban_hour[]" id="ban_hour_10" type="checkbox" value="10"'.($ban_hour_arr[10] == 1 ? ' checked' : '').'><label for="ban_hour_10">10</label></td>
          <td><input name="ban_hour[]" id="ban_hour_11" type="checkbox" value="11"'.($ban_hour_arr[11] == 1 ? ' checked' : '').'><label for="ban_hour_11">11</label></td>
          <td><input name="ban_hour[]" id="ban_hour_12" type="checkbox" value="12"'.($ban_hour_arr[12] == 1 ? ' checked' : '').'><label for="ban_hour_12">12</label></td>
          <td><input name="ban_hour[]" id="ban_hour_13" type="checkbox" value="13"'.($ban_hour_arr[13] == 1 ? ' checked' : '').'><label for="ban_hour_13">13</label></td>
          <td><input name="ban_hour[]" id="ban_hour_14" type="checkbox" value="14"'.($ban_hour_arr[14] == 1 ? ' checked' : '').'><label for="ban_hour_14">14</label></td>
          <td><input name="ban_hour[]" id="ban_hour_15" type="checkbox" value="15"'.($ban_hour_arr[15] == 1 ? ' checked' : '').'><label for="ban_hour_15">15</label></td>
         </tr>
         <tr>
          <td><input name="ban_hour[]" id="ban_hour_16" type="checkbox" value="16"'.($ban_hour_arr[16] == 1 ? ' checked' : '').'><label for="ban_hour_16">16</label></td>
          <td><input name="ban_hour[]" id="ban_hour_17" type="checkbox" value="17"'.($ban_hour_arr[17] == 1 ? ' checked' : '').'><label for="ban_hour_17">17</label></td>
          <td><input name="ban_hour[]" id="ban_hour_18" type="checkbox" value="18"'.($ban_hour_arr[18] == 1 ? ' checked' : '').'><label for="ban_hour_18">18</label></td>
          <td><input name="ban_hour[]" id="ban_hour_19" type="checkbox" value="19"'.($ban_hour_arr[19] == 1 ? ' checked' : '').'><label for="ban_hour_19">19</label></td>
          <td><input name="ban_hour[]" id="ban_hour_20" type="checkbox" value="20"'.($ban_hour_arr[20] == 1 ? ' checked' : '').'><label for="ban_hour_20">20</label></td>
          <td><input name="ban_hour[]" id="ban_hour_21" type="checkbox" value="21"'.($ban_hour_arr[21] == 1 ? ' checked' : '').'><label for="ban_hour_21">21</label></td>
          <td><input name="ban_hour[]" id="ban_hour_22" type="checkbox" value="22"'.($ban_hour_arr[22] == 1 ? ' checked' : '').'><label for="ban_hour_22">22</label></td>
          <td><input name="ban_hour[]" id="ban_hour_23" type="checkbox" value="23"'.($ban_hour_arr[23] == 1 ? ' checked' : '').'><label for="ban_hour_23">23</label></td>
         </tr>
        </table>

       </td>
      </tr>
      <tr>
       <td>Исключить показы<br/>в указанные дни<br/>недели</td>
       <td>

        <table>
         <tr>
          <td><input name="ban_week_day[]" id="ban_week_day_1" type="checkbox" value="1"'.($ban_week_day_arr[1] == 1 ? ' checked' : '').'><label for="ban_week_day_1">Понедельник</label></td>
          <td><input name="ban_week_day[]" id="ban_week_day_2" type="checkbox" value="2"'.($ban_week_day_arr[2] == 1 ? ' checked' : '').'><label for="ban_week_day_2">Вторник</label></td>
          <td><input name="ban_week_day[]" id="ban_week_day_3" type="checkbox" value="3"'.($ban_week_day_arr[3] == 1 ? ' checked' : '').'><label for="ban_week_day_3">Среда</label></td>
          <td><input name="ban_week_day[]" id="ban_week_day_4" type="checkbox" value="4"'.($ban_week_day_arr[4] == 1 ? ' checked' : '').'><label for="ban_week_day_4">Четверг</label></td>
         </tr>
         <tr>
          <td><input name="ban_week_day[]" id="ban_week_day_5" type="checkbox" value="5"'.($ban_week_day_arr[5] == 1 ? ' checked' : '').'><label for="ban_week_day_5">Пятница</label></td>
          <td><input name="ban_week_day[]" id="ban_week_day_6" type="checkbox" value="6"'.($ban_week_day_arr[6] == 1 ? ' checked' : '').'><label for="ban_week_day_6">Суббота</label></td>
          <td><input name="ban_week_day[]" id="ban_week_day_0" type="checkbox" value="0"'.($ban_week_day_arr[0] == 1 ? ' checked' : '').'><label for="ban_week_day_0">Воскресенье</label></td>
          <td></td>
         </tr>
        </table>

       </td>
      </tr>

      <tr>
       <td>Добавлять метку<br/>в URL объявлений</td>
       <td>
         <div style="width: 350px;">
            <select name="labels" class="select">
              <option value="">Нет </option>
              <option value="_utm"'.($labels == '_utm' ? ' selected' : '').'>Utm </option>
              <option value="_openstat"'.($labels == '_openstat' ? ' selected' : '').'>Openstat </option>
              <option value="_from"'.($labels == '_from' ? ' selected' : '').'>From </option>
              <option value="_subid"'.($labels == '_subid' ? ' selected' : '').'>SubID </option>
            </select>
		    </div>
       </td>
      </tr>
      
      <tr>
       <td>Метка для SubID<br/>{tiz_id} - id тизера<br/>{source} - id источника</td>
       <td style="vertical-align: middle;">
         <div style="width: 250px;">
          <input name="subid_mark" type="text" value="'.(!empty($subid) ? $subid : "").'" maxlength="255" class="inp">
		    </div>
       </td>
      </tr>

      <tr>
       <td></td>
       <td>
        <input name="postnewcampaign" type="hidden" value="post"><input type="submit" value="Сохранить" class="btn">
        <input type="button" value="Отмена" class="btn" onclick="document.location.href=\'/advertiser/campaigns/\'">
       </td>
      </tr>
     </table>
    </form>
	';

	return $txt;
}

function show_campaign_change_status(){
	global $base;
	global $var;

	$status = get_int_val('status');
	$campaign_id = get_int_val('campaign_id');

	if($status == 2){
		$campaign = $base->exec('select count(*)
		from '.$var['base_tab_prefix'].'campaigns
		where user_id = "'.$var['user_id'].'"
		and id = "'.$campaign_id.'"
		and status = "3"
		');

		if($campaign[0][0] == 1){
			$base->exec('update '.$var['base_tab_prefix'].'campaigns
			set status = "'.$status.'"
			where id="'.$campaign_id.'"');

			$base->exec('update '.$var['base_tab_prefix'].'teasers
			set status = "'.$status.'"
			where campaign_id="'.$campaign_id.'"');
		}

	}elseif($status == 3){
		$campaign = $base->exec('select count(*)
		from '.$var['base_tab_prefix'].'campaigns
		where user_id = "'.$var['user_id'].'"
		and id = "'.$campaign_id.'"
		and status = "2"
		');

		if($campaign[0][0] == 1){
			$base->exec('update '.$var['base_tab_prefix'].'campaigns
			set status = "'.$status.'"
			where id="'.$campaign_id.'"');

			$base->exec('update '.$var['base_tab_prefix'].'teasers
			set status = "'.$status.'"
			where campaign_id="'.$campaign_id.'"');
		}
	}

	@header('location: /advertiser/campaigns/');
	exit;
}

function show_campaign_remove(){
	global $base;
	global $var;

	$campaign_id = get_int_val('campaign_id');

	$campaign = $base->exec('select count(*)
	from '.$var['base_tab_prefix'].'campaigns
	where user_id = "'.$var['user_id'].'"
	and id = "'.$campaign_id.'"
	');

	if($campaign[0][0] == 1){
		$base->exec('delete from '.$var['base_tab_prefix'].'campaigns
		where user_id = "'.$var['user_id'].'"
		and id = "'.$campaign_id.'"');

		$teasers = $base->exec('select id, image from '.$var['base_tab_prefix'].'teasers
		where user_id = "'.$var['user_id'].'"
		and campaign_id = "'.$campaign_id.'"');

		if(count($teasers) > 0){
			foreach($teasers as $teaser){
				@unlink('../../li/'.$teaser['image']);

				$base->exec('delete from '.$var['base_tab_prefix'].'teasers
				where user_id = "'.$var['user_id'].'"
				and id = "'.$teaser['id'].'"');
			}
		}
	}

	@header('location: /advertiser/campaigns/');
	exit;
}
?>