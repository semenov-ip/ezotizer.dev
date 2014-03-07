<?php
include('../includes/sql.class.inc');
include('../includes/utilites.inc');
include('../cnf.inc');
include('../includes/pattern.inc');
include('../includes/news.inc');
include('../includes/menu.inc');

if(get_str_val('action') == 'add' && $var['user_admin']){
	$body = show_news_add();

}elseif(get_str_val('action') == 'edit' && get_int_val('news_id') && $var['user_admin']){
	$body = show_news_add();

}else{
	$body = show_news();
}

$pattern = Array(
	'pattern'=>'../patterns/index.html',
	'menu'=>show_menu(),
	'title'=> $var['title'],
	'body'=>$body
);

echo show_pattern($pattern);
?>
