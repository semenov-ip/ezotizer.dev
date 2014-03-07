<?php
function show_top10(){
	global $base;
	global $var;

	$txt .= '
	<h1>ТОП-10 вебмастеров</h1>
    <p></p>
	<table class="list">
	 <tr class="head">
	  <td>Логин</td>
	  <td>Доход</td>
	 </tr>
	';

	$webmaster = $base->exec('select id
	from '.$var['base_tab_prefix'].'users
	where notshow_top10_login = "0"');

	if(count($webmaster) == 0){
		$error = 1;
	}

	if(!$error){
		foreach($webmaster as $webmaster){
		   	if($webmasters_ids == ''){
	 	  		$webmasters_ids = $webmaster['id'];
	  		}else{
	   			$webmasters_ids .= ','.$webmaster['id'];
	   		}
		}

		if($webmasters_ids == ''){
			$error = 1;
		}
	}

	if(!$error){
		$site = $base->exec('select id
		from '.$var['base_tab_prefix'].'sites
		where user_id in ('.$webmasters_ids.')');

		if(count($site) == 0){
			$error = 1;
		}
	}

	if(!$error){
		foreach($site as $site){
	    	if($sites_ids == ''){
	    		$sites_ids = $site['id'];

	    	}else{
	    		$sites_ids .= ','.$site['id'];
	    	}
		}

		if($sites_ids == ''){
			$error = 1;
		}
	}

	if(!$error){
		$top = $base->exec('select site_id, sum(count_rur) as count_rur
		from '.$var['base_tab_prefix'].'sites_stat
		where site_id in ('.$sites_ids.')
		and dataadd = "'.$var['date'].'"
		and count_rur > 0
		group by site_id');

		if(count($top) == 0){
			$error = 1;
		}
	}

	if(!$error){
		$i = 0;
		foreach($top as $top){
        	$arr[$top['site_id']] = $top['count_rur'];
        	$i++;
		}

		asort($arr, SORT_NUMERIC);

		$num = 1;

		foreach($arr as $variable => $value){
			$site = $base->exec('select user_id
			from '.$var['base_tab_prefix'].'sites
			where id = '.$variable.'');

            $webmasters[$site[0]['user_id']] += $value;

			/*$num++;

			if($num == 11){
				break;
			} */

			/*$tr = '
				<tr>
				 <td>'.$webmaster[0]['top10_login'].'</td>
				 <td>'.$user_count.'</td>
				</tr>
				'.$tr;

			$webmaster = $base->exec('select top10_login
			from '.$var['base_tab_prefix'].'users
			where id = "'.$site[0]['user_id'].'"');

			if($webmaster[0]['top10_login'] == ''){
				$user_login = 'Вебмастер#'.$site[0]['user_id'];
			}
				*/
		}

        arsort($webmasters, SORT_NUMERIC);

		foreach($webmasters as $variable => $value){
			$webmaster = $base->exec('select top10_login
			from '.$var['base_tab_prefix'].'users
			where id = "'.$variable.'"');

			if($webmaster[0]['top10_login'] == ''){
				$webmaster[0]['top10_login'] = 'Вебмастер#'.$site[0]['user_id'];
			}

			$txt .= '
			<tr>
			 <td>'.$webmaster[0]['top10_login'].'</td>
			 <td>'.$value.'</td>
			</tr>
			';
		}

		$txt .= $tr;
	}

	if($error){
		$txt .= '
		<tr>
		 <td colspan="2" align="center">список пуст.</td>
		</tr>
		';
	}

	$txt .= '
	<tr>
	 <td colspan="2" class="footer">&nbsp;</td>
	</tr>
	';

	$txt .= '
	</table>
	';

	return $txt;
}
?>