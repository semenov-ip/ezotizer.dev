<?php
@include('../includes/utilites.inc');
@include('../includes/sql.class.inc');
@include('../cnf.inc');

$txt = '';
$block_id = get_int_val('block_id');
$site_referer = get_str_val('r');
$start = get_int_val('start');


if(!isset($_SERVER['HTTP_REFERER']))exit;
$referer = $_SERVER['HTTP_REFERER'];

// если нету удалённого порта, вырубаем показ тизеров
if($_SERVER['REMOTE_PORT'] == ''){
	echo 'var block = document.getElementById(\'teaser_'.$block_id.'\'); var text = \'#3\'; if(block){block.innerHTML = text;}';
	exit;
}

// если не определяется браузер юзера, вырубаем показ тизеров
if($_SERVER['HTTP_USER_AGENT'] == ''){
	echo 'var block = document.getElementById(\'teaser_'.$block_id.'\'); var text = \'#4\'; if(block){block.innerHTML = text;}';
	exit;
}

$referer_url = '';

//// выдираем адрес сайта из ссылки
if(substr($referer, 0 , 7) == 'http://'){
	$referer_url = substr($referer, 7);
}

$referer_url_pos = strpos($referer_url, '/');

if($referer_url_pos){
	$referer_url = substr($referer_url, 0, $referer_url_pos);
}

if(substr($referer_url, 0 , 4) == 'www.'){
	$referer_url = substr($referer_url, 4);
}

if(!$block_id || !$referer){
	echo 'var block = document.getElementById(\'teaser_'.$block_id.'\'); var text = \'#5\'; if(block){block.innerHTML = text;}';
	exit;
}

@include('../includes/ipgeobase.inc');

$block = $base->exec('select user_id, site_id, block_size_num, block_size_value, hor, ver, size, size_x, size_y, position, align, font_family, font_size, font_color, font_color_hover, background_color,
table_border_width, table_border_color, table_border_style, image_border_width, image_border_color, image_border_style, cell_background, cell_margin, cell_padding, cell_border_width, cell_border_color, cell_border_style,
show_partner_lnk
from '.$var['base_tab_prefix'].'blocks
where id = '.$block_id.'');

if(count($block) != 1){
	echo 'var block = document.getElementById(\'teaser_'.$block_id.'\'); var text = \'#6\'; if(block){block.innerHTML = text;}';
	exit;
}

$limit = $block[0]['hor'] * $block[0]['ver'];

$site_id = $block[0]['site_id'];
$webmaster_id = $block[0]['user_id'];

$site = $base->exec('select url, aliases, ban_teaser, url_coding, sections, status
from '.$var['base_tab_prefix'].'sites
where id = '.$site_id.'
and user_id = '.$webmaster_id.'');

if(count($site) == 1 && $site[0]['status'] == '0'){
	$base->exec('update '.$var['base_tab_prefix'].'sites
	set status = 1
	where id = '.$site_id.'');
}

if(count($site) != 1 || $site[0]['status'] != '2'){
	echo 'var block = document.getElementById(\'teaser_'.$block_id.'\'); var text = \'#7\'; if(block){block.innerHTML = text;}';
	exit;
}

if(!strstr($site[0]['url'], $referer_url) && !strstr($site[0]['aliases'], $referer_url)){
	echo 'var block = document.getElementById(\'teaser_'.$block_id.'\'); var text = \'#8\'; if(block){block.innerHTML = text;}';
	exit;
}

$base->exec('update '.$var['base_tab_prefix'].'sites
set last_load = "'.$var['datetime'].'"
where id = '.$site_id.'');

if($site[0]['url_coding'] == 'utf8'){
	$base->exec('set names utf8');
}elseif($site[0]['url_coding'] == 'koi8r'){
	$base->exec('set names koi8r');
  
}

//////////////// получаем список запрещенных тематик для сайта
$ban_sections_ids = '';

$section = $base->exec('select id, name
from '.$var['base_tab_prefix'].'sections
order by binary(name) asc');

foreach($section as $section){
	if(!strstr($site[0]['sections'], '~'.$section['id'].'~')){
		if($ban_sections_ids == ''){
			$ban_sections_ids = $section['id'];

		}else{
            $ban_sections_ids .= ','.$section['id'];
		}
	}
}

/////////////// получаем данные для геотаргетинга
$ipgeobase = ipgeobase($var['user_ip']);

if($ipgeobase['region'] == '-' && $site[0]['url_coding'] == 'utf8')$ipgeobase['region'] = iconv("windows-1251", "utf-8", 'Прочие регионы');
if($ipgeobase['country'] == '-' && $site[0]['url_coding'] == 'utf8')$ipgeobase['country'] = iconv("windows-1251", "utf-8", 'Прочие страны');
if($ipgeobase['region'] == '-' && $site[0]['url_coding'] == 'koi8r')$ipgeobase['region'] = iconv("windows-1251", "koi8-r", 'Прочие регионы');
if($ipgeobase['country'] == '-' && $site[0]['url_coding'] == 'koi8r')$ipgeobase['country'] = iconv("windows-1251", "koi8-r", 'Прочие страны');
if($ipgeobase['region'] == '-' && $site[0]['url_coding'] == 'cp1251')$ipgeobase['region'] = 'Прочие регионы';
if($ipgeobase['country'] == '-' && $site[0]['url_coding'] == 'cp1251')$ipgeobase['country'] = 'Прочие страны';

/////////////// формируем список тизеров
$teaser = $base->exec('select id, user_id, campaign_id, image, text
from '.$var['base_tab_prefix'].'teasers_list
where ban_site not like "%'.$referer_url.'%"
'.($site[0]['ban_teaser'] != '' ? 'and id not in ('.$site[0]['ban_teaser'].')' : '').'
'.($ban_sections_ids ? 'and section_id not in ('.$ban_sections_ids.')' : '').'
'.($ipgeobase['region'] != '' ? 'and ban_region not like "%~'.$ipgeobase['region'].'~%"' : '').'
'.($ipgeobase['country'] != '' ? 'and ban_country not like "%~'.$ipgeobase['country'].'~%"' : '').'
'.(isset($_COOKIE['teaser_ids']) && $_COOKIE['teaser_ids'] != '' ? 'and id not in('.$_COOKIE['teaser_ids'].')' : '').'
and url not like "%'.$referer_url.'%"
order by firstord asc, secondord desc, last_show asc
limit '.($start ? $start.', ' : '').''.$limit);

if(isset($_COOKIE['teaser_ids']) && $_COOKIE['teaser_ids'] != ''){
	$cookie_arr = $_COOKIE['teaser_ids'];
}

if(count($teaser) == 0 || count($teaser) < $limit){
	$teaser_cnt = count($teaser);

	$new_teaser = $base->exec('select id, user_id, campaign_id, image, text
	from '.$var['base_tab_prefix'].'teasers_list
	where ban_site not like "%'.$referer_url.'%"
	'.($site[0]['ban_teaser'] != '' ? 'and id not in ('.$site[0]['ban_teaser'].')' : '').'
	'.($ban_sections_ids ? 'and section_id not in ('.$ban_sections_ids.')' : '').'
	'.($ipgeobase['region'] != '' ? 'and ban_region not like "%~'.$ipgeobase['region'].'~%"' : '').'
	'.($ipgeobase['country'] != '' ? 'and ban_country not like "%~'.$ipgeobase['country'].'~%"' : '').'
	and url not like "%'.$referer_url.'%"
	order by firstord asc, secondord desc, last_show asc
	limit '.($limit - $teaser_cnt));

	if(count($new_teaser) > 0){
		foreach($new_teaser as $new_teaser){
        	$teaser[] = $new_teaser;
		}
	}

	$not_add_cookie = true;
	@setcookie('teaser_ids', '', time() + 43200, '/');
}

if(count($teaser) != 0){
	$txt .= '<table id="teaser_block_table_'.$block_id.'"><tr>';

	///// переменные
	$http_user_agent = $_SERVER['HTTP_USER_AGENT'];
	$remote_addr = $_SERVER['REMOTE_ADDR'];
	$remote_port = $_SERVER['REMOTE_PORT'];
	$http_x_forwarded_for = '';

	if(isset($_SERVER['HTTP_X_FORWARDED_FOR'])){
		$http_x_forwarded_for = $_SERVER['HTTP_X_FORWARDED_FOR'];
	}

	$teaser_user_hash = md5($http_user_agent.$remote_addr);
	
	$num = 0;
  
  // Если у ссылки больше блоков на странице, отключаем показ у всех остальных
  save_stat('site', $site_id);
  save_stat('block', $block_id);
  
  foreach($teaser as $teaser){
    $log = $base->exec('select id
      from '.$var['base_tab_prefix'].'logs_'.date("Y_m_d").'
      where
      block_id = '.$block_id.'
      and teaser_id = '.$teaser['id'].'
      and user_hash = "'.$teaser_user_hash.'"
      and dataadd = "'.$var['date'].'"
		');

		$hash = md5($teaser['user_id'].$site_id.$block_id.$teaser['campaign_id'].$teaser['id'].$teaser_user_hash.$var['date']);

		if(count($log) == 0){
			$base->exec('insert into '.$var['base_tab_prefix'].'logs_'.date("Y_m_d").'(
        advertiser_id,
        webmaster_id,
        site_id,
        block_id,
        campaign_id,
        teaser_id,
        user_hash,
        user_ip,
        user_agent,
        user_city,
        user_region,
        user_country,
        view,
        uview,
        click,
        uclick,
        dataadd,
        hash
      )values(
        '.$teaser['user_id'].',
        '.$block[0]['user_id'].',
        '.$site_id.',
        '.$block_id.',
        '.$teaser['campaign_id'].',
        '.$teaser['id'].',
        "'.$teaser_user_hash.'",
        "'.$var['user_ip'].'",
        "'.$http_user_agent.'",
        "'.$ipgeobase['city'].'",
        "'.$ipgeobase['region'].'",
        "'.$ipgeobase['country'].'",
        1,
        1,
        0,
        0,
        "'.$var['date'].'",
        "'.$hash.'"
			)');

			$log[0]['user_id'] = count($user_cnt) + 1;

			save_stat('campaign', $teaser['campaign_id']);
			save_stat('teaser', $teaser['id']);
		} else {
	    $base->exec('update '.$var['base_tab_prefix'].'logs_'.date("Y_m_d").'
	      set view = (view + 1)
	      where id = '.$log[0]['id'].'');

	    update_stat('campaign', $teaser['campaign_id']);
	    update_stat('teaser', $teaser['id']);
		}

		$click_url = 'http://'.$_SERVER['SERVER_NAME'].'/click.php?teaser_id='.$teaser['id'].'&hash='.$hash.'&country='.$ipgeobase['country'];

		$base->exec('update '.$var['base_tab_prefix'].'teasers
		set
		last_show = "'.$var['datetime'].'"
		where id = '.$teaser['id'].'');

		$base->exec('update '.$var['base_tab_prefix'].'teasers_list
		set
		last_show = "'.$var['datetime'].'"
		where id = '.$teaser['id'].'');
		if($num >= $block[0]['hor']){
			$num = 0;
			$txt .= '</tr><tr>';
			$br = '<br/>';
		}

		$txt .= '<td id="teaser_block_td"><div id="teaser_block_div">';
		$txt .= '<a href="'.$click_url.'" target="_blank">';
		$txt .= '<img id="teaser_block_img" src="http://'.$_SERVER['SERVER_NAME'].'/li/'.$teaser['image'].'"></a>';
		$txt .= ''.($block[0]['position'] == 'top' ? '<br/>' : '').'';
		$txt .= '<a href="'.$click_url.'" target="_blank">'.cleanTextTeaser($teaser['text']).'</a>';
		$txt .= '</div></td>';

		$num ++;
		$br = '';

		if(!isset($cookie_arr)){
			$cookie_arr = $teaser['id'];

		}else{
			$cookie_arr .= ','.$teaser['id'];
		}
	}

	$txt .= '</tr>';

	if($block[0]['show_partner_lnk'] == 1){
		$partner_lnk_wrd = 'Добавить объявление';

		if($site[0]['url_coding'] == 'utf8'){
			$partner_lnk_wrd = iconv("windows-1251", "utf-8", 'Добавить объявление');

		}elseif($site[0]['url_coding'] == 'koi8r'){
			$partner_lnk_wrd = iconv("windows-1251", "koi8-r", 'Добавить объявление');
		}

		$txt .= '<tr><td colspan="'.$block[0]['hor'].'" id="teaser_block_partner_lnk"><a href="http://'.$_SERVER['SERVER_NAME'].'/?pid='.$block[0]['user_id'].'" target="_blank">'.$partner_lnk_wrd.'</a></td></tr>';
	}

	$txt .= '</table>';
	$txt .= '<style>';
	$txt .= '#teaser_block_table_'.$block_id.'{font-family: '.$block[0]['font_family'].'; width: '.$block[0]['block_size_num'].''.$block[0]['block_size_value'].'; border-collapse: separate !important; border-spacing: '.$block[0]['cell_margin'].'px; border: '.($block[0]['table_border_width'] == 'none' ? '0' : $block[0]['table_border_width']).'px '.$block[0]['table_border_style'].' '.$block[0]['table_border_color'].'; background-color: '.$block[0]['background_color'].';}';
	$txt .= ' #teaser_block_table_'.$block_id.' #teaser_block_header{text-align: left !important; padding: 0.3em 0.5em !important; background: '.$block[0]['cell_background'].';}';
	$txt .= ' #teaser_block_table_'.$block_id.' #teaser_block_td{border: '.$block[0]['cell_border_width'].'px '.$block[0]['cell_border_color'].' '.$block[0]['cell_border_style'].'; text-align: '.$block[0]['align'].' !important; vertical-align: top !important; width: '.round(100 / $block[0]['hor']).'%; background: '.$block[0]['cell_background'].'; padding: '.$block[0]['cell_padding'].'px !important;}';
	$txt .= ' #teaser_block_table_'.$block_id.' #teaser_block_div a{color: '.$block[0]['font_color'].' !important; font-size: '.$block[0]['font_size'].' !important;}';
	$txt .= ' #teaser_block_table_'.$block_id.' #teaser_block_div a:hover{color: '.$block[0]['font_color_hover'].' !important;}';
	$txt .= ' #teaser_block_table_'.$block_id.' #teaser_block_div #teaser_block_img{'.($block[0]['position'] != 'top' ? ' float: '.$block[0]['position'].'; ' : '').'width: '.$block[0]['size'].'px; height: '.$block[0]['size'].'px; border: '.($block[0]['image_border_width'] == 'none' ? '0' : $block[0]['image_border_width']).'px '.$block[0]['image_border_style'].' '.$block[0]['image_border_color'].'; '.($block[0]['position'] == 'left' ? 'margin: 0 '.$block[0]['cell_padding'].'px 0 0' : ($block[0]['position'] == 'right' ? 'margin: 0 0 0 '.$block[0]['cell_padding'].'px' : 'margin: 0 0 0 0')).';}';
	$txt .= ' #teaser_block_table_'.$block_id.' #teaser_block_partner_lnk{text-align: right; padding: inherit;}';
	$txt .= ' #teaser_block_table_'.$block_id.' #teaser_block_partner_lnk a{color: inherit !important;}';
	$txt .= ' #teaser_block_table_'.$block_id.' #teaser_block_partner_lnk a:hover{color: inherit !important;}';
	$txt .= '</style>';

	if(isset($cookie_arr) && $cookie_arr != '' && !isset($not_add_cookie)){
		@setcookie('teaser_ids', $cookie_arr, time() + 43200, '/');
	}
	
	if($site[0]['url_coding'] == 'utf8'){
		$txt = iconv('utf-8','windows-1251', $txt);
	}
	
	echo 'var block = document.getElementById(\'teaser_'.$block_id.'\'); var text = \''.$txt.'\'; block.innerHTML = text;';
}else{
	echo 'var block = document.getElementById(\'teaser_'.$block_id.'\'); var text = \'#9\'; if(block){block.innerHTML = text;}';
}

function save_stat($table, $field){
	global $base;
	global $var;
	
  $stat = $base->exec('select id from '.$var['base_tab_prefix'].''.$table.'s_stat
	where '.$table.'_id = "'.$field.'" and dataadd = "'.$var['date'].'"');

  if( count($stat) == 0 ){
    $base->exec('insert into '.$var['base_tab_prefix'].''.$table.'s_stat (
      '.$table.'_id,
      view,
      uview,
      click,
      uclick,
      count_rur,
      dataadd
    ) values (
      "'.$field.'",
      1,
      1,
      0,
      0,
      0.00,
      "'.$var['date'].'"
    )');
  } else {
    $base->exec('update '.$var['base_tab_prefix'].''.$table.'s_stat
      set view = (view + 1), uview = (uview + 1)
      where '.$table.'_id = '.$field.' and dataadd = "'.$var['date'].'"');
	}
}

function update_stat($table, $field){
	global $base;
	global $var;
	$base->exec('update '.$var['base_tab_prefix'].''.$table.'s_stat
	set view = (view + 1)
	where '.$table.'_id = '.$field.' and dataadd = "'.$var['date'].'"');
}

function cleanTextTeaser($teaserText){
    //$teaserText = preg_replace("/\n|\r|\r\n|(\r\n)+/u", "", $teaserText);
    //$teaserText = preg_replace('/[\s]{2,}/', ' ', $teaserText);

    return $teaserText;
}
?>
