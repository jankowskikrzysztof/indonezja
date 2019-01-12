<?php

$mysql_login = 'root';
$mysql_password = '7622522';
$mysql_database = 'scubafroggy';

$dbh = new PDO('mysql:host=localhost;dbname='.$mysql_database, $mysql_login, $mysql_password, array( PDO::ATTR_PERSISTENT => false));

?>
