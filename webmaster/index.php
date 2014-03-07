<?php
include('../includes/sql.class.inc');
include('../includes/utilites.inc');
include('../cnf.inc');

if(!$var['user_id']){
	@header('location: /login/?return_page='.urlencode($_SERVER['REQUEST_URI']));
	exit;
}

$base->exec('update '.$var['base_tab_prefix'].'users
set who="webmaster"
where id="'.$var['user_id'].'"');

@header('location: ./sites/');
exit;
?>
