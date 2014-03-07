<?php
function ipgeobase($ip){
	global $base;
	global $var;

	if(!isset($_COOKIE['user_city'])){
		$iparr = explode(".", $ip);
		$longip = 16777216 * $iparr[0] + 65536 * $iparr[1] + 256 * $iparr[2] + $iparr[3];

		$ipgeobase = $base->exec('select country, city, region
		from '.$var['base_tab_prefix'].'ipgeobase
		where
		min_longip <= '.$longip.'
		and max_longip >= '.$longip.'
		order by max_longip asc');

		if(count($ipgeobase) != 0){
			$txt['country'] = $ipgeobase[0]['country'];
			$txt['region'] = $ipgeobase[0]['region'];
			$txt['city'] = $ipgeobase[0]['city'];

		}else{
			$txt['country'] = '-';
			$txt['region'] = '-';
			$txt['city'] = '-';
		}

		@setcookie('user_city', $txt['city'], time() + 2592000, '/');
		@setcookie('user_region', $txt['region'], time() + 2592000, '/');
		@setcookie('user_country', $txt['country'], time() + 2592000, '/');

	}else{
		$txt['country'] = $_COOKIE['user_country'];
		$txt['region'] = $_COOKIE['user_region'];
		$txt['city'] = $_COOKIE['user_city'];
	}

	return $txt;
}
?>