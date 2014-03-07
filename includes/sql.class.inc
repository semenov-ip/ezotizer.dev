<?php
class SQLclass {
	#-- Constructor
	function SQLclass( $host, $database, $user, $password ) {
		$this->_database = $database;
		$this->_host	 = $host;
		$this->_user	 = $user;
		$this->_password = $password;
		$this->_dbc		 = @mysql_connect( $this->_host, $this->_user, $this->_password );
	}

	#-- записи в массив состоящий из ассоциативных массивов записей
	function exec( $query, $nofetch=false ) {
		@mysql_select_db( $this->_database, $this->_dbc );
		$qry = @mysql_query( $query, $this->_dbc );
		$ret = array();
		if ($qry>0 && $nofetch==false) {
		 if (@mysql_num_rows($qry)>0) while ($row=mysql_fetch_array($qry)) $ret[]=$row;
		}
		@mysql_free_result($qry);
		return $ret;
	}

	#-- записи в массив просых нумерованных массивов
	function exec_ar( $query, $nofetch=false ) {
		mysql_select_db( $this->_database, $this->_dbc );
		$qry = mysql_query( $query, $this->_dbc );
		$ret = array();
		if ($qry>0 && $nofetch==false) {
		 if (mysql_num_rows($qry)>0) while ($row=mysql_fetch_row($qry)) $ret[]=$row;
		}
		@mysql_free_result($qry);
		return $ret;
	}

	#-- записи в массив объектов
	function exec_obj( $query, $nofetch=false ) {
		mysql_select_db( $this->_database, $this->_dbc );
		$qry = mysql_query( $query, $this->_dbc );
		$ret = array();
		if ($qry>0 && $nofetch==false) {
		 if (mysql_num_rows($qry)>0) while ($row=mysql_fetch_object($qry)) $ret[]=$row;
		}
		@mysql_free_result($qry);
		return $ret;
	}

	#-- закрыть соединение с базой
	function close() {
		mysql_select_db( $this->_database, $this->_dbc );
		mysql_close( $this->_dbc );
	}

	#-- id последней добавленной записи
	function new_id() {
		return mysql_insert_id( $this->_dbc );
	}

	#-- затронуто записей последним запросом
	function affected() {
		return mysql_affected_rows( $this->_dbc );
	}

}
?>