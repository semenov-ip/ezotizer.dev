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
include('../../includes/settings.inc');

$body = show_advertiser_settings();

$pattern = Array(
	'pattern'	=> '../../patterns/index.html',
	'menu'		=> show_menu(),
	'title'		=> $var['title'],
	'body'		=> show_account_menu().show_navigation_menu().show_navigation_menu_advertiser().$body
);

echo show_pattern($pattern);
?>
