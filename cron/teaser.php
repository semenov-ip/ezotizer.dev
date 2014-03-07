<?php
@include('../includes/utilites.inc');
@include('../includes/sql.class.inc');
@include('../cnf.inc');

/////////////// получаем список кампаний для определения списка рекламодателей
$all_campaigns = $base->exec('select user_id
from '.$var['base_tab_prefix'].'campaigns');

foreach($all_campaigns as $all_campaigns){
	if($campaigns_users_ids == ''){
		$campaigns_users_ids = $all_campaigns['user_id'];

	}else{
		$campaigns_users_ids .= ','.$all_campaigns['user_id'];
    
	}
}

/////////////// получаем список рекламодателей с положительным балансом (более 3 рублей)
$good_users = $base->exec('select id
from '.$var['base_tab_prefix'].'users
where id in ('.$campaigns_users_ids.')
and count_rur > 3');

foreach($good_users as $good_users){
	if($good_users_ids == ''){
		$good_users_ids = $good_users['id'];

	}else{
		$good_users_ids .= ','.$good_users['id'];
	}
}


$teasers = $base->exec('select id, user_id, campaign_id, section_id, ban_site, ban_region, ban_country, ban_hour, ban_week_day, image, url, text, last_show,
case
when teaserstat.view <=> null then 0
when teaserstat.view >= 0 and teaserstat.view <= 5000 and teaserstat.click = 0 then 1
when teaserstat.click > 0 then 2
when teaserstat.view > 5000 then 2
end as firstord,
case
when teaserstat.view is not null and teaserstat.click = 0 then 0.01
when teaserstat.view is not null then (teaserstat.ctr * teaserstat.ctr)
end as secondord
from '.$var['base_tab_prefix'].'teasers left join (
select teaser_id, view, click, ((100/view)*click) as ctr
from '.$var['base_tab_prefix'].'teasers_stat
where dataadd >= "'.date("Y-m-d", (strtotime($var['date']) - 86400)).'"
) as teaserstat
on '.$var['base_tab_prefix'].'teasers.id = teaserstat.teaser_id
where '.($good_users_ids != '' ? 'user_id in ('.$good_users_ids.')' : 'user_id = "bla-bla"').'
and ban_hour not like "%~'.date("G", strtotime($var['datetime'])).'~%"
and ban_week_day not like "%~'.date("w", strtotime($var['datetime'])).'~%"
and status = "2"
order by firstord asc, secondord desc, last_show asc');

$base->exec('truncate table '.$var['base_tab_prefix'].'teasers_list');

$i = 1;
if(count($teasers) != 0){
 	foreach($teasers as $teaser){
    $base->exec('insert into '.$var['base_tab_prefix'].'teasers_list
    	(
      id,
      user_id,
      campaign_id,
      section_id,
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
      currentpos,
      firstord,
      secondord
    	)values(
      "'.$teaser['id'].'",
      "'.$teaser['user_id'].'",
      "'.$teaser['campaign_id'].'",
      "'.$teaser['section_id'].'",
      "'.$teaser['ban_site'].'",
      "'.$teaser['ban_region'].'",
      "'.$teaser['ban_country'].'",
      "'.$teaser['ban_hour'].'",
      "'.$teaser['ban_week_day'].'",
      "'.$teaser['image'].'",
      "'.$teaser['url'].'",
      "'.$teaser['text'].'",
      "0000-00-00 00:00:00",
      "0000-00-00 00:00:00",
      "'.$teaser['last_show'].'",
      "'.$i.'",
      "'.$teaser['firstord'].'",
      "'.(!empty($teaser['secondord']) ? $teaser['secondord'] : "00.0").'"
    	)');

    	echo $teaser['id'].' - '.'<br/>';
    	$i++;
 	}

 	$base->exec('optimize table '.$var['base_tab_prefix'].'teasers_list');
}

?>