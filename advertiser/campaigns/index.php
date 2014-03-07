<?php
include('../../includes/sql.class.inc');
include('../../includes/utilites.inc');
include('../../cnf.inc');

if(!$var['user_id']){
	@header('location: /login/?return_page='.urlencode($_SERVER['REQUEST_URI']));
	exit;
}

if(!$var['user_admin']){
	@header('location: /');
	exit;
}

include('../../includes/pattern.inc');
include('../../includes/menu.inc');
include('../../includes/campaigns.inc');
include('../../includes/teasers.inc');
include('../../includes/advertiser_stat.inc');
include('../../includes/campaigns_stat.inc');
include('../../includes/chart_object.inc');
include('../../includes/chart.inc');

if(get_str_val('action') == 'data'){
	show_stat_data();
	exit;

}elseif(get_str_val('action') == 'remove' && get_int_val('teaser_id')){
	$body = show_teaser_remove();

}elseif(get_str_val('action') == 'change_status' && get_int_val('teaser_id')){
	$body = show_teaser_change_status();

}elseif(get_str_val('action') == 'editteaser' && get_int_val('campaign_id') && get_int_val('teaser_id')){
	$body = show_teaser_add();

}elseif(get_str_val('action') == 'addteaser' && get_int_val('campaign_id')){
	$body = show_teaser_add();

}elseif(get_str_val('action') == 'teasers' && get_int_val('campaign_id')){
	$body = show_teasers();

}elseif(get_str_val('action') == 'remove' && get_int_val('campaign_id')){
	$body = show_campaign_remove();

}elseif(get_str_val('action') == 'change_status' && get_int_val('campaign_id')){
	$body = show_campaign_change_status();

}elseif(get_str_val('action') == 'edit' && get_int_val('campaign_id')){
	$body = show_campaign_add();

}elseif(get_str_val('action') == 'add'){
	$body = show_campaign_add();

}else{
	$body = show_campaigns();
}

$pattern = Array(
	'pattern'	=> '../../patterns/index.html',
	'menu'		=> show_menu(),
	'title'		=> $var['title'],
	'body'		=> show_account_menu().show_navigation_menu().show_navigation_menu_advertiser().$body
);

echo show_pattern($pattern);
?>
