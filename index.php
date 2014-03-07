<?php
include('includes/sql.class.inc');
include('includes/utilites.inc');
include('cnf.inc');
include('includes/pattern.inc');
include('includes/auth.inc');
include('includes/menu.inc');

if($var['user_id']){
	@header('location: /'.$var['user_who'].'/');

}else{
	$body = show_auth_form();
}

$pattern = Array(
	'pattern'=>'patterns/index.html',
	'menu'=>show_menu(),
	'title'=> $var['title'],
	'body'=>$body
);

echo show_pattern($pattern);
?>