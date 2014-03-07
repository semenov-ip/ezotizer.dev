<?php
include('../../includes/sql.class.inc');
include('../../includes/utilites.inc');
include('../../cnf.inc');

if(!$var['user_id']){
	@header('location: /login/?return_page='.urlencode($_SERVER['REQUEST_URI']));
	exit;
}

include('../../includes/pattern.inc');
include('../../includes/menu.inc');
include('../../includes/sites.inc');
include('../../includes/blocks.inc');
include('../../includes/webmaster_stat.inc');
include('../../includes/campaigns_stat.inc');
include('../../includes/chart_object.inc');
include('../../includes/chart.inc');

if(get_str_val('action') == 'data'){
	show_stat_data();
	exit;

}elseif(get_str_val('action') == 'remove' && get_int_val('block_id')){
	$body = show_block_remove();

}if(get_str_val('action') == 'editblock' && get_int_val('site_id') && get_int_val('block_id')){
	$body = show_block_add();

}elseif(get_str_val('action') == 'blocks' && get_int_val('site_id')){
	$body = show_blocks();

}elseif(get_str_val('action') == 'addblock' && get_int_val('site_id')){
	$body = show_block_add();

}elseif(get_str_val('action') == 'remove' && get_int_val('site_id')){
	$body = show_site_remove();

}elseif(get_str_val('action') == 'change_status' && get_int_val('site_id')){
	$body = show_site_change_status();

}elseif(get_str_val('action') == 'add' || get_str_val('action') == 'edit'){
	$body = show_site_add();

}else{
	$body = show_sites();
}

$pattern = Array(
	'pattern'	=> '../../patterns/index.html',
	'menu'		=> show_menu(),
	'title'		=> $var['title'],
	'body'		=> show_account_menu().show_navigation_menu().show_navigation_menu_webmaster().$body
);

echo show_pattern($pattern);
?>
