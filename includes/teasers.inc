<?php
function show_teasers(){
	global $base;
	global $var;

	$campaign_id = get_int_val('campaign_id');

	$status = array(
		0 => 'новый',
		1 => 'на модерации',
		2 => 'идут показы',
		3 => 'показы<br/>приостановлены',
		4 => 'заблокирован'
	);
  
  $txt .= '
	<div class="path2">
	 <img src="/i/path.gif" width="16" height="16" align="absmiddle">
	 <a href="/advertiser/">Для рекламодателя</a> /
	 <a href="/advertiser/campaigns/">Рекламные кампании</a> /
	 <b>'.$campaign[0]['name'].'</b>
	</div>';
  
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
      <table width=40% align="right" style="margin-bottom:10px">
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

	$campaign = $base->exec('select name from '.$var['base_tab_prefix'].'campaigns
	where user_id = "'.$var['user_id'].'"
	and id = "'.$campaign_id.'"');

	if(count($campaign) != 1){
		@header('location: /advertiser/campaigns/');
		exit;
	}

	$txt .= '
	<div class="section_menu">
	 <a href="./?action=addteaser&campaign_id='.$campaign_id.'"><img src="/i/add.gif" width="16" height="16" align="absmiddle">Добавить объявление</a>
	</div>
	';

	$txt .= '
	<table class="list">
	 <tr class="head">
	  <td>#id</td>
	  <td>Изображение</td>
	  <td>Текст</td>
	  <td>Статус</td>
	  <td>Эффективность показов</td>
	  <td>Показы</td>
	  <td>Клики</td>
	  <td>CTR</td>
	  <td>Расход</td>
	  <td></td>
	</tr>
	';

	$teaser = $base->exec('select id, section_id, image, url, text, status
	from '.$var['base_tab_prefix'].'teasers
	where
	user_id="'.$var['user_id'].'"
	and campaign_id="'.$campaign_id.'"
	order by dataadd desc');

  $campaignStatistics = campaignsStatExtratc($teaser, 'teasers_stat', 'teaser_id');
  
  $view_cnt = 0;
	$click_cnt = 0;

	if(count($campaignStatistics) > 0){
		$num = 0;

		foreach($campaignStatistics as $teaser){
			$stat = $base->exec('select sum(view) as view, sum(click) as click, sum(count_rur) as count_rur
			from '.$var['base_tab_prefix'].'teasers_stat
			where teaser_id = "'.$teaser['id'].'"');

			$view = $teaser['view'];
			$click = $teaser['click'];
			$ctr = $teaser['ctr'];
			$count_rur = $teaser['count_rur'];

			$pos_teaser_list = $base->exec('select id
			from '.$var['base_tab_prefix'].'teasers_list
			order by firstord asc, secondord desc, last_show asc');

			$line = 0;
	    $top = false;

			if(count($pos_teaser_list) > 0){
				foreach($pos_teaser_list as $pos_teaser){
          $line += 1;

          if($pos_teaser['id'] == $teaser['id']){
            $top = 1;
            break;
          }
				}
			}

			if(!$top){
				$line = 0;
			}

			if($line == 0){
        $indicator = 'battery_red.gif';

			}elseif($line > 0 && $line <= 10){
        $indicator = 'battery_green.gif';

			}elseif($line > 10 && $line <= 20){
				$indicator = 'battery_orange.gif';

			}elseif($line > 20 && $line <= 30){
				$indicator = 'battery_renge.gif';

			}elseif($line > 30 && $line <= 40){
				$indicator = 'battery_orange.gif';

			}elseif($line > 40){
				$indicator = 'battery_red.gif';
			}

			$txt .= '
		    <tr'.($num?' class="even"':'').'>
			 <td>'.$teaser['id'].'</td>
			 <td><a href="'.html_entity_decode($teaser['url']).'" target="_blank"><img src="/li/'.$teaser['image'].'" width="70" height="70"></a></td>
			 <td><a href="./?action=editteaser&campaign_id='.$campaign_id.'&teaser_id='.$teaser['id'].'">'.$teaser['text'].'</a></td>
			 <td>'.$status[$teaser['status']].'</td>
			 <td><img src="/i/'.$indicator.'"></td>
			 <td>'.$view.'</td>
			 <td>'.$click.'</td>
			 <td>'.$ctr.'</td>
			 <td>'.$count_rur.'</td>
			 <td align="right" nowrap>
			';

			if($teaser['status'] == 2){
                $txt .= '
				<img src="/i/play_notactive.gif" width="16" height="16" align="absmiddle" alt="Начать показ объявления" title="Начать показ объявления">
				<a href="./?action=change_status&teaser_id='.$teaser['id'].'&status=3"><img src="/i/pause_active.gif" width="16" height="16" align="absmiddle" alt="Приостановить показ объявления" title="Приостановить показ объявления"></a>
				';

			}elseif($teaser['status'] == 3){
                $txt .= '
				<a href="./?action=change_status&teaser_id='.$teaser['id'].'&status=2"><img src="/i/play_active.gif" width="16" height="16" align="absmiddle" alt="Начать показ объявления" title="Начать показ объявления"></a>
				<img src="/i/pause_notactive.gif" width="16" height="16" align="absmiddle" alt="Приостановить показ объявления" title="Приостановить показ объявления">
				';
			}

			$txt .= '
			  <a href="./?action=editteaser&campaign_id='.$campaign_id.'&teaser_id='.$teaser['id'].'"><img src="/i/edit.gif" width="16" height="16" align="absmiddle" alt="Редактировать объявление" title="Редактировать объявление"></a>
			  <a href="./?action=remove&teaser_id='.$teaser['id'].'"><img src="/i/delete.gif" width="16" height="16" align="absmiddle" alt="Удалить объявление" title="Удалить объявление" onclick="conf(\'Вы уверены, что хотите удалить объявление?\', this); return false;"></a>
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
		  <td colspan="11" align="center">список пуст.</td>
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

function show_teaser_add(){
	global $base;
	global $var;

	$campaign_id = get_int_val('campaign_id');
	$teaser_id = get_int_val('teaser_id');

	$campaign = $base->exec('select name, section_id, ban_site, ban_region, ban_country,
    ban_hour, ban_week_day
    from '.$var['base_tab_prefix'].'campaigns
    where user_id = "'.$var['user_id'].'"
    and id = "'.$campaign_id.'"'
  );

	if(count($campaign) != 1){
		@header('location: /advertiser/campaigns/');
		exit;
	}

	if($teaser_id){
		$teaser = $base->exec('select campaign_id, section_id, image, url, text
		from '.$var['base_tab_prefix'].'teasers
		where
		user_id="'.$var['user_id'].'"
		and id="'.$teaser_id.'"');

		if(count($teaser) != 1){
			@header('location: /advertiser/campaigns/');
			exit;
		}

		$campaign_id = $teaser[0]['campaign_id'];
		$section_id = $teaser[0]['section_id'];
		$text = $teaser[0]['text'];
		$image = $teaser[0]['image'];
		$url = $teaser[0]['url'];
	}

	if($_POST['postnewteaser'] == 'post'){
		$section_id = post_int_val('section_id');
    $name = stripslashes(htmlspecialchars(trim(post_str_val('name'))));

		$text = stripslashes(trim(post_str_val('text')));
		$url = stripslashes(htmlspecialchars(trim(post_str_val('url'))));

		$src = $_FILES['image']['tmp_name'];
		$src_type = $_FILES['image']['type'];
		$size = @getimagesize($src);
		$image_postfix = substr(md5($var['datetime']), 0, 3);
		$image_type = Array(
			'image/gif'=>'.gif',
			'image/jpg'=>'.jpg',
			'image/jpeg'=>'.jpeg',
			'image/png'=>'.png'
		);

		if(substr($url, 0, 7) != 'http://'){
			$url = 'http://'.$url;
		}

		if(!$error && !$teaser_id){
			if(!$src){
				$error = 'не выбрано изображение.';
			}
		}

		if(!$error && $src){
			if(!strstr($src_type, 'gif')
				&& !strstr($src_type, 'jpeg')
				&& !strstr($src_type, 'jpg')
				&& !strstr($src_type, 'png'))
			{
				$error = 'не допустимый тип изображения.';
			}
		}

		if(!$error && $src){
			if(filesize($src) > $var['max_image_file_size'] * 1024){
   			$error = 'не допустимый размер изображения. Размер должен быть не более '.$var['max_image_file_size'].' кб';
      }
		}

		if(!$error && $src){
      if(!@copy($src, '../../li/'.$campaign_id.'-'.$image_postfix.''.$image_type[$src_type])){
        $error = 'не удалось сохранить изображение.';
      }
		}

		if(!$error){
			if(!$text){
				$error = 'не указан текст.';
			}
		}

		if(!$error){
			if(strlen($text) > 120){
				$error = 'текст описания больше 120 символов.';
				$text = substr($text, 0, 120);
			}
		}

		if($error){
			@unlink('../../li/'.$campaign_id.'-'.$image_postfix.''.$image_type[$src_type]);
		}

		if(!$error){
			if($teaser_id){
				$base->exec('update '.$var['base_tab_prefix'].'teasers
				set
				section_id = "'.$section_id.'",
				'.($src ? 'image="'.$campaign_id.'-'.$teaser_id.'-'.$image_postfix.''.$image_type[$src_type].'",' : '').'
         url="'.$url.'",
         text="'.$text.'",
         dataedit="'.$var['datetime'].'",
         status="2"
         where id="'.$teaser_id.'"');

				if($src){
					@unlink('../../li/'.$image.'');
	          @rename('../../li/'.$campaign_id.'-'.$image_postfix.''.$image_type[$src_type], '../../li/'.$campaign_id.'-'.$teaser_id.'-'.$image_postfix.''.$image_type[$src_type]);
					$image = $campaign_id.'-'.$teaser_id.'-'.$image_postfix.''.$image_type[$src_type];
	      }
        $postok = 'изменения сохранены.';

			}else{
				$base->exec('
						insert into '.$var['base_tab_prefix'].'teasers(
						user_id,
						section_id,
						campaign_id,
						ban_site,
						ban_region,
						ban_country,
						ban_hour,
						ban_week_day,
						image,
						url,
						text,
						dataadd,
						dataedit,
						last_show,
						status
						)values(
						"'.$var['user_id'].'",
						"'.$section_id.'",
						"'.$campaign_id.'",
						"'.$campaign[0]['ban_site'].'",
						"'.$campaign[0]['ban_region'].'",
						"'.$campaign[0]['ban_country'].'",
						"'.$campaign[0]['ban_hour'].'",
						"'.$campaign[0]['ban_week_day'].'",
						"'.$campaign_id.'-'.$image_postfix.''.$image_type[$src_type].'",
						"'.$url.'",
						"'.$text.'",
						"'.$var['datetime'].'",
						"'.$var['datetime'].'",
						"0000-00-00 00:00:00",
						"2"
						)
	        	');
	      
        $id = $base->new_id();
				if($id){
					@rename('../../li/'.$campaign_id.'-'.$image_postfix.''.$image_type[$src_type], '../../li/'.$campaign_id.'-'.$id.'-'.$image_postfix.''.$image_type[$src_type]);

					$base->exec('update '.$var['base_tab_prefix'].'teasers
					set
					image="'.$campaign_id.'-'.$id.'-'.$image_postfix.''.$image_type[$src_type].'"
					where id="'.$id.'"');

					@header('location: ./?action=editteaser&campaign_id='.$campaign_id.'&teaser_id='.$id.'');
					exit;

				}else{
					$error = 'не удалось сохранить объявление. Попробуйте позже.';
					@unlink('../../li/'.$campaign_id.'-'.$image_postfix.''.$image_type[$src_type]);
				}
			}
		}
	}



	$txt .= '
	<div class="path2">
	 <img src="/i/path.gif" width="16" height="16" align="absmiddle">
	 <a href="/advertiser/">Для рекламодателя</a> /
	 <a href="/advertiser/campaigns/">Рекламные кампании</a> /
	 <a href="/advertiser/campaigns/?action=teasers&campaign_id='.$campaign_id.'">'.$campaign[0]['name'].'</a> /
	 <b>'.($teaser_id ? 'Объявление #'.$teaser_id : 'Новое объявление').'</b>
	</div>';

	if($teaser_id){
		$txt .= '<h1>Редактирование объявления</h1>';

	}else{
		$txt .= '<h1>Добавление объявления</h1>';
	}

	if($error){
		$txt .= '<div class="posterror"><img src="/i/error.gif" width="16" height="16" align="absmiddle"> <b>Ошибка:</b> '.$error.'</div>';
	}

	if($postok){
		$txt .= '<div class="postok"><img src="/i/ok.gif" width="16" height="16" align="absmiddle"> '.$postok.'</div>';
	}

	$txt .='
	<form action="'.$_SERVER['REQUEST_URI'].'" method="post" enctype="multipart/form-data">
	 <table class="form">
      <tr>
       <td>Изображение</td>
       <td>
        '.($image ? '<img src="/li/'.$image.'" style="margin-bottom: 5px;">' : '').'
        <div style="width: 350px;"><input name="image" type="file" class="inp"></div>
        <div class="dsc">Тип - gif, jpg, png;<br/>Размер - не более '.$var['max_image_file_size'].' кб.</div>
       </td>
      </tr>
      <tr>
       <td>Ссылка</td>
       <td>
        <div style="width: 350px;"><input name="url" type="text" value="'.($url ? $url : 'http://').'" maxlength="255" class="inp"></div>
       </td>
      </tr>
      <tr>
       <td>Текст</td>
       <td>
        <div style="width: 350px;"><textarea name="text" id="text" onKeyUp="check_len(\'text\', 120)">'.$text.'</textarea></div>
        <div class="dsc">Максимум 75 символов. Осталось: <span class="red b" id="text_indicator">'.(120 - strlen($text)).'</span></div>
       </td>
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
		$txt .= '<option value="'.$sections['id'].'"'.($sections['id'] == $section_id ? ' selected' : (!$section_id && $campaign[0]['section_id'] == $sections['id']? ' selected' : '')).'>'.$sections['name'].' </option>';
	}

  	$txt .='
		</select>
        </div>
       </td>
      </tr>
       <td></td>
       <td><input name="postnewteaser" type="hidden" value="post"><input type="submit" value="Сохранить" class="btn"></td>
      </tr>
	 </table>
	</form>
	';

	return $txt;
}

function show_teaser_change_status(){
	global $base;
	global $var;

	$status = get_int_val('status');
	$teaser_id = get_int_val('teaser_id');

	if($status == 2){
		$teaser = $base->exec('select campaign_id
		from '.$var['base_tab_prefix'].'teasers
		where user_id = "'.$var['user_id'].'"
		and id = "'.$teaser_id.'"
		and status = "3"
		');

		if(count($teaser) == 1){
			$base->exec('update '.$var['base_tab_prefix'].'teasers
			set status = "'.$status.'"
			where id="'.$teaser_id.'"');
		}

	}elseif($status == 3){
		$teaser = $base->exec('select campaign_id
		from '.$var['base_tab_prefix'].'teasers
		where user_id = "'.$var['user_id'].'"
		and id = "'.$teaser_id.'"
		and status = "2"
		');

		if(count($teaser) == 1){
			$base->exec('update '.$var['base_tab_prefix'].'teasers
			set status = "'.$status.'"
			where id="'.$teaser_id.'"');
		}
	}

	@header('location: /advertiser/campaigns/?action=teasers&campaign_id='.$teaser[0]['campaign_id'].'');
	exit;
}

function show_teaser_remove(){
	global $base;
	global $var;

	$teaser_id = get_int_val('teaser_id');

	$teaser = $base->exec('select campaign_id, image
	from '.$var['base_tab_prefix'].'teasers
	where user_id = "'.$var['user_id'].'"
	and id = "'.$teaser_id.'"
	');

	if(count($teaser) == 1){
		$base->exec('delete from '.$var['base_tab_prefix'].'teasers
		where user_id = "'.$var['user_id'].'"
		and id = "'.$teaser_id.'"');

        @unlink('../../li/'.$teaser[0]['image'].'');
	}

	@header('location: /advertiser/campaigns/?action=teasers&campaign_id='.$teaser[0]['campaign_id'].'');
	exit;
}
?>