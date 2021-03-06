<?php
include('../includes/sql.class.inc');
include('../includes/utilites.inc');
include('../cnf.inc');

if(!$var['user_id']){
	@header('location: /login/?return_page='.urlencode($_SERVER['REQUEST_URI']));
	exit;
}

$action = get_str_val('action');

if(!$action){
	@header('location: ./?action=free');
	exit;
}

include('../includes/pattern.inc');
include('../includes/menu.inc');
include('../includes/invitations.inc');

$body = show_invitations();

$pattern = Array(
	'pattern'	=> '../patterns/index.html',
	'menu'		=> show_menu(),
	'title'		=> $var['title'],
	'body'		=> show_account_menu().show_navigation_menu().show_navigation_menu_invitations().$body
);

echo show_pattern($pattern);
?>
