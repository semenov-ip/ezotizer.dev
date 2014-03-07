<?php
include('../includes/sql.class.inc');
include('../includes/utilites.inc');
include('../cnf.inc');
include('../includes/pattern.inc');
include('../includes/pages.inc');
include('../includes/menu.inc');

$body = show_page('faq');

$pattern = Array(
	'pattern'=>'../patterns/index.html',
	'menu'=>show_menu(),
	'title'=> $var['title'],
	'body'=>$body
);

echo show_pattern($pattern);
?>
