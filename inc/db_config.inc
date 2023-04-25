<?php

$mysql_login = '';
$mysql_password = '';
$mysql_database = 'scubafroggy';

$dbh = new PDO('mysql:host=localhost;dbname='.$mysql_database, $mysql_login, $mysql_password, array( PDO::ATTR_PERSISTENT => false));

?>
