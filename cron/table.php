<?php
@include('../includes/utilites.inc');
@include('../includes/sql.class.inc');
@include('../cnf.inc');

$table_end = date("Y_m_d", time() + 86400);

//////////////// создаём таблицу для логов
$table = $base->exec('show tables');
foreach($table as $table){
	if($table[0] == $var['base_tab_prefix'].'logs_'.$table_end){
		$table_exists = true;
	}
}

if(!$table_exists){
	$base->exec('create table '.$var['base_tab_prefix'].'logs_'.$table_end.' (
	id bigint( 19 ) NOT NULL AUTO_INCREMENT ,
	advertiser_id int( 7 ) NOT NULL ,
	webmaster_id int( 7 ) NOT NULL ,
	site_id int( 7 ) NOT NULL ,
	block_id int( 7 ) NOT NULL ,
	campaign_id int( 7 ) NOT NULL ,
	teaser_id int( 7 ) NOT NULL ,
	user_hash varchar( 255 ) NOT NULL ,
	user_ip varchar( 15 ) NOT NULL ,
	user_agent varchar( 255 ) NOT NULL ,
	user_city varchar( 255 ) NOT NULL ,
	user_region varchar( 255 ) NOT NULL ,
	user_country varchar( 255 ) NOT NULL ,
	view int( 7 ) NOT NULL ,
	uview int( 7 ) NOT NULL ,
	click int( 7 ) NOT NULL ,
	uclick int( 7 ) NOT NULL ,
	dataadd date NOT NULL ,
	hash varchar( 255 ) NOT NULL ,
	primary key ( block_id , teaser_id , user_hash , dataadd ) ,
	unique key id ( id ),
	key hash ( hash )
	) engine = innoDB default charset = utf8;
	');
}
?>