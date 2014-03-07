<?php
include('../includes/sql.class.inc');
include('../includes/utilites.inc');
include('../cnf.inc');

if(!$var['user_id']){
	@header('location: /login/?return_page='.urlencode($_SERVER['REQUEST_URI']));
	exit;
}

$action = get_str_val('action');

include('../includes/pattern.inc');
include('../includes/menu.inc');
include('../includes/top10.inc');

$body = show_top10();

$pattern = Array(
	'pattern'	=> '../patterns/index.html',
	'menu'		=> show_menu(),
	'title'		=> $var['title'],
	'body'		=> show_account_menu().$body
);

echo show_pattern($pattern);
?>
