<?php
@include('includes/utilites.inc');
@include('includes/sql.class.inc');
@include('cnf.inc');

$hash = get_str_val('hash');
$country = get_str_val('country');

if(!$hash){
	@header('location: /');
	exit;
}

$log = $base->exec('select id, advertiser_id, webmaster_id, site_id, block_id, campaign_id, teaser_id, user_hash
from '.$var['base_tab_prefix'].'logs_'.date("Y_m_d").'
where hash="'.$hash.'"
');

if(count($log) != 1){
	@header('location: /');
	exit;

}else{
	for($i = 0; $i <= 1; $i++){
		$click_cnt = $base->exec('select sum(click) as click
		from '.$var['base_tab_prefix'].'logs_'.date("Y_m_d", (time() - (86400 * $i))).'
		where teaser_id="'.$log[0]['teaser_id'].'"
		and user_hash="'.$log[0]['user_hash'].'"');
		
		if($click_cnt[0]['click'] != 0 && $click_cnt[0]['click'] != ''){
			$not_unique_click = true;
		}
	}
	
	$not_unique_click = false;
	
	$teaser = $base->exec('select url
	from '.$var['base_tab_prefix'].'teasers
	where id = '.$log[0]['teaser_id'].'');
	
	$site = $base->exec('select price, price_cis
	from '.$var['base_tab_prefix'].'sites
	where id = '.$log[0]['site_id'].'');
	
	$price = countryPrice($site, $country);
  $site_price = true;
	
	if($not_unique_click != true){
		//////// списываем средства с баланса рекламодателя за клик
		$base->exec('update '.$var['base_tab_prefix'].'users
			set
			count_rur = (count_rur - '.float_to_mysql($price).')
			where id = "'.$log[0]['advertiser_id'].'"');
		
		//////// вносим информацию в историю баланса рекламодателя
		$history = $base->exec('select id
			from '.$var['base_tab_prefix'].'count_history
			where user_id = "'.$log[0]['advertiser_id'].'"
			and text = "Списание за клики"
			and date(dataadd) = "'.$var['date'].'"');

		if(count($history) == 0){
			$base->exec('insert into '.$var['base_tab_prefix'].'count_history
			(
				user_id,
				amount,
				text,
				trans_type,
				dataadd,
				status
			)values(
				"'.$log[0]['advertiser_id'].'",
				"'.float_to_mysql($price).'",
				"Списание за клики",
				"0",
				"'.$var['datetime'].'",
				"1"
			)');

		}else{
			$base->exec('update '.$var['base_tab_prefix'].'count_history
				set amount = (amount + '.float_to_mysql($price).')
				where id = "'.$history[0]['id'].'"');
		}

		if(isset($site_price) && $price > 0){
			///////// зачисляем средства на баланс вебмастера за клик
			$base->exec('update '.$var['base_tab_prefix'].'users
			set
			count_rur = (count_rur + '.float_to_mysql((isset($site_price) ? $price : ($price * $var['webmaster_procent']))).')
			where id = "'.$log[0]['webmaster_id'].'"');
			
			//////// вносим информацию в историю баланса вебмастера
			$history = $base->exec('select id
			from '.$var['base_tab_prefix'].'count_history
			where user_id = "'.$log[0]['webmaster_id'].'"
			and text = "Зачисление за клики"
			and date(dataadd) = "'.$var['date'].'"');

			if(count($history) == 0){
				$base->exec('insert into '.$var['base_tab_prefix'].'count_history
				(
					user_id,
					amount,
					text,
					trans_type,
					dataadd,
					status
				)values(
					"'.$log[0]['webmaster_id'].'",
					"'.float_to_mysql((isset($site_price) ? $price : ($price * $var['webmaster_procent']))).'",
					"Зачисление за клики",
					"1",
					"'.$var['datetime'].'",
					"1"
				)');

			}else{
	      $base->exec('update '.$var['base_tab_prefix'].'count_history
	        set amount = (amount + '.float_to_mysql((isset($site_price) ? $price : ($price * $var['webmaster_procent']))).')
	        where id = "'.$history[0]['id'].'"');
			}
		}

		if(!isset($site_price)){
			///////// зачисляем средства на баланс партнера пригласившего вебмастера
			$webmaster = $base->exec('select pid
			from '.$var['base_tab_prefix'].'users
			where id = "'.$log[0]['webmaster_id'].'"');

			if($webmaster[0]['pid'] != 0){
				$base->exec('update '.$var['base_tab_prefix'].'users
				set
				count_rur = (count_rur + '.float_to_mysql(($price - ($price * $var['webmaster_procent'])) * $var['partner_procent']).')
				where id = "'.$webmaster[0]['pid'].'"');

				//////// вносим информацию в историю баланса партнера
				$history = $base->exec('select id
				from '.$var['base_tab_prefix'].'count_history
				where user_id = "'.$webmaster[0]['pid'].'"
				and text = "Партнерское вознаграждение за вебмастера"
				and date(dataadd) = "'.$var['date'].'"');

				if(count($history) == 0){
					$base->exec('insert into '.$var['base_tab_prefix'].'count_history
					(
						user_id,
						amount,
						text,
						trans_type,
						dataadd,
						status
					)values(
						"'.$webmaster[0]['pid'].'",
						"'.float_to_mysql(($price - ($price * $var['webmaster_procent'])) * $var['partner_procent']).'",
						"Партнерское вознаграждение за вебмастера",
						"1",
						"'.$var['datetime'].'",
						"1"
					)');

				}else{
		            $base->exec('update '.$var['base_tab_prefix'].'count_history
		            set amount = (amount + '.float_to_mysql(($price - ($price * $var['webmaster_procent'])) * $var['partner_procent']).')
		            where id = "'.$history[0]['id'].'"');
				}

				//////////// обновляем статистику по доходам от реферала
				$stat = $base->exec('select id
				from '.$var['base_tab_prefix'].'partners_stat
				where user_id = "'.$log[0]['webmaster_id'].'"
				and partner_id = "'.$webmaster[0]['pid'].'"
				and dataadd = "'.$var['date'].'"');

			   	if(count($stat) == 0){
			   		$base->exec('insert into '.$var['base_tab_prefix'].'partners_stat
			   		(
			   		user_id,
			   		partner_id,
			   		count_rur,
			   		dataadd
			   		) values (
			   		"'.$log[0]['webmaster_id'].'",
			   		"'.$webmaster[0]['pid'].'",
			   		"'.float_to_mysql(($price - ($price * $var['webmaster_procent'])) * $var['partner_procent']).'",
			   		"'.$var['date'].'"
			   		)');

			   	}else{
			        $base->exec('update '.$var['base_tab_prefix'].'partners_stat
					set count_rur = (count_rur + '.float_to_mysql(($price - ($price * $var['webmaster_procent'])) * $var['partner_procent']).')
					where user_id = "'.$log[0]['webmaster_id'].'"
					and partner_id="'.$webmaster[0]['pid'].'"
					and dataadd = "'.$var['date'].'"');
				}
			}

			///////// зачисляем средства на баланс партнера пригласившего рекламодателя
			$advertiser = $base->exec('select pid
			from '.$var['base_tab_prefix'].'users
			where id = "'.$log[0]['advertiser_id'].'"');

			if($advertiser[0]['pid'] != 0){
				$base->exec('update '.$var['base_tab_prefix'].'users
				set
				count_rur = (count_rur + '.float_to_mysql(($price - ($price * $var['webmaster_procent'])) * $var['partner_procent']).')
				where id = "'.$advertiser[0]['pid'].'"');

				//////// вносим информацию в историю баланса партнера
				$history = $base->exec('select id
				from '.$var['base_tab_prefix'].'count_history
				where user_id = "'.$advertiser[0]['pid'].'"
				and text = "Партнерское вознаграждение за рекламодателя"
				and date(dataadd) = "'.$var['date'].'"');

				if(count($history) == 0){
					$base->exec('insert into '.$var['base_tab_prefix'].'count_history
					(
						user_id,
						amount,
						text,
						trans_type,
						dataadd,
						status
					)values(
						"'.$advertiser[0]['pid'].'",
						"'.float_to_mysql(($price - ($price * $var['webmaster_procent'])) * $var['partner_procent']).'",
						"Партнерское вознаграждение за рекламодателя",
						"1",
						"'.$var['datetime'].'",
						"1"
					)');
				}else{
					$base->exec('update '.$var['base_tab_prefix'].'count_history
					set amount = (amount + '.float_to_mysql(($price - ($price * $var['webmaster_procent'])) * $var['partner_procent']).')
					where id = "'.$history[0]['id'].'"');
				}

				//////////// одновляем статистику по доходам от реферала
				$stat = $base->exec('select id
				from '.$var['base_tab_prefix'].'partners_stat
				where user_id = "'.$log[0]['advertiser_id'].'"
				and partner_id = "'.$advertiser[0]['pid'].'"
				and dataadd = "'.$var['date'].'"');

				if(count($stat) == 0){
					$base->exec('insert into '.$var['base_tab_prefix'].'partners_stat
					(
					user_id,
					partner_id,
					count_rur,
					dataadd
					) values (
					"'.$log[0]['advertiser_id'].'",
					"'.$advertiser[0]['pid'].'",
					"'.float_to_mysql(($price - ($price * $var['webmaster_procent'])) * $var['partner_procent']).'",
					"'.$var['date'].'"
					)');

				}else{
					$base->exec('update '.$var['base_tab_prefix'].'partners_stat
						set count_rur = (count_rur + '.float_to_mysql(($price - ($price * $var['webmaster_procent'])) * $var['partner_procent']).')
						where user_id = "'.$log[0]['advertiser_id'].'"
						and partner_id="'.$advertiser[0]['pid'].'"
						and dataadd = "'.$var['date'].'"');
				}
			}
		}

		$base->exec('update '.$var['base_tab_prefix'].'logs_'.date("Y_m_d").'
			set
			click = "1",
			uclick = "1"
			where id = "'.$log[0]['id'].'"');
		
		save_stat('campaign', $log[0]['campaign_id'], float_to_mysql($price));
		save_stat('teaser', $log[0]['teaser_id'], float_to_mysql($price));
		save_stat('site', $log[0]['site_id'], float_to_mysql($price));
		save_stat('block', $log[0]['block_id'], float_to_mysql($price));

	} else {
    $base->exec('update '.$var['base_tab_prefix'].'logs_'.date("Y_m_d").'
			set
			click = (click + 1)
			where id = "'.$log[0]['id'].'"');

		update_stat('campaign', $log[0]['campaign_id']);
		update_stat('teaser', $log[0]['teaser_id']);
		update_stat('site', $log[0]['site_id']);
		update_stat('block', $log[0]['block_id']);
	}
	$click_url = html_entity_decode($teaser[0]['url']);
	
	$label_type = $base->exec('select labels, subid
	from '.$var['base_tab_prefix'].'campaigns
	where id = "'.$log[0]['campaign_id'].'"');

	$subid = $label_type[0]['subid'];
	$label_type = $label_type[0]['labels'];
	
	if($label_type != ''){
		if(strstr($click_url, '?')){
			$label_url_start = '&';

		}else{
			$label_url_start = '?';
		}

		$label_medium = $base->exec('select id, url
		from '.$var['base_tab_prefix'].'sites
		where id = "'.$log[0]['site_id'].'"');

		$label_medium = $label_medium[0]['id'];

		if($label_type == '_utm'){
			$label_url = 'utm_source='.$var['pay_title'].'&utm_medium='.$label_medium.'&utm_campaign='.$log[0]['campaign_id'].'&utm_content='.$log[0]['teaser_id'].'';

		}elseif($label_type == '_openstat'){
			$label_params = base64_encode(''.$var['pay_title'].';'.$log[0]['campaign_id'].';'.$log[0]['teaser_id'].';'.$label_medium.'');
			$label_url = '_openstat='.$label_params;

		}elseif($label_type == '_from'){
			$label_url = 'from='.$var['pay_title'].'';
			
		}elseif($label_type == "_subid"){
			if(!empty($subid)){
				$label_url = subid_designer( $subid,$log[0]['teaser_id'],$log[0]['site_id'] );
			}else {
				$label_url = 'sub1='.$log[0]['teaser_id'].';&sub2='.$log[0]['site_id'].'';
			}
		}
	}
	
	if($label_type == "_subid"){
		@header('location: '.$click_url.$label_url);
	}else{
		@header('location: '.$click_url.$label_url_start.$label_url);
	}
}

function save_stat($table, $field, $count){
	global $base;
	global $var;

	$base->exec('update '.$var['base_tab_prefix'].''.$table.'s_stat
	set
	click = (click + 1),
	uclick = (uclick + 1),
	count_rur = count_rur + '.$count.'
	where '.$table.'_id="'.$field.'" and dataadd = "'.$var['date'].'"');
}

function update_stat($table, $field){
	global $base;
	global $var;

	$base->exec('update '.$var['base_tab_prefix'].''.$table.'s_stat
	set
	click = (click + 1)
	where '.$table.'_id="'.$field.'" and dataadd = "'.$var['date'].'"');
}

function countryPrice($dataPrice, $country){
	if($country=="RU"){
		return $dataPrice[0]['price'];
	} else {
		return $dataPrice[0]['price_cis'];
	}
}

function subid_designer($subid, $teaser_id,$site_id){

	$subid_rename = str_replace("{tiz_id}", $teaser_id, $subid);

	$subid_rename = str_replace("{source}", $site_id, $subid_rename);
	
	return $subid_rename;
}
?>