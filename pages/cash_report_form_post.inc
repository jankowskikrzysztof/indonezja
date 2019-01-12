<?php

$post_id = $_POST[id];
$cash_book_group_id = $_POST[cash_book_group_id];
$location_id = $_POST[location_id];
$to_location_id = $_POST[to_location_id];
$personel_id = $_SESSION[user_id];
$activity_id = 0;
$type = $_POST[type];
$date = $_POST[date];
$desc = $_POST[desc];
$cash_in = str_replace(',','',$_POST[cash_in]);
$cash_out = str_replace(',','',$_POST[cash_out]);
$currency = $_POST[currency];




if($_POST['source_action']=='add' and $_SESSION['permission']=='rw')
    {
    $stmt = $dbh->prepare("INSERT INTO `cash_book`
                                  (`cash_book_group_id`,
                                   `location_id`,
                                   `personel_id`,
                                   `cash_report_only`,
                                   `type`,
                                   `date`,
                                   `desc`,
                                   `pay_cash`,
                                   `value`,
                                   `currency`)

                           VALUES ('".$cash_book_group_id."',
                                   '".$location_id."',
                                   '".$personel_id."',
                                   '1',
                                   '".$type."',
                                   '".$date."',
                                   '".$desc."',
                                   '".$cash_in."',
                                   '".$cash_out."',
                                   '".$currency."')
                           ");
    $stmt -> execute();
    echo '<p>Data add success</p>';
    $last_insert_id = $dbh->lastInsertId();

    if($cash_book_group_id==22 and $type==2) // DO USUNIECIA PO WYSTARTOWANIU cash movement
	{

	$cash_cr_in = $cash_out;

	$stmt_cr_loc = $dbh->prepare("SELECT location.name as loc_name,location.id_location
		                      FROM location
		                      WHERE location.id_location = ".$location_id."
		                      ORDER BY location.name");
	$stmt_cr_loc -> execute();
	$location_cr_array = $stmt_cr_loc->fetchAll();
        $location_cr_name = $location_cr_array[0][loc_name];

	$stmt = $dbh->prepare("INSERT INTO `cash_book`
		                          (`cash_book_group_id`,
					   `cash_book_source`,
		                           `location_id`,
		                           `personel_id`,
		                           `cash_report_only`,
		                           `type`,
		                           `date`,
		                           `desc`,
		                           `pay_cash`,
		                           `value`,
		                           `currency`)

		                   VALUES ('22',
					   '".$last_insert_id."',
		                           '13',
		                           '".$personel_id."',
		                           '1',
		                           '1',
		                           '".$date."',
		                           'from ".$location_cr_name."',
		                           '".$cash_cr_in."',
		                           '".$cash_cr_out."',
		                           '".$currency."')
		                   ");
	$stmt -> execute();
	echo '<p>Data add success</p>';
	$last_insert_id = $dbh->lastInsertId();
	} // DO USUNIECIA PO WYSTARTOWANIU cash movement

    if($cash_book_group_id==23 and $type==2) // DO USUNIECIA PO WYSTARTOWANIU cash movement
	{

	$cash_cr_in = $cash_out;

	$stmt_cr_loc = $dbh->prepare("SELECT location.name as loc_name,location.id_location
		                      FROM location
		                      WHERE location.id_location = ".$location_id."
		                      ORDER BY location.name");
	$stmt_cr_loc -> execute();
	$location_cr_array = $stmt_cr_loc->fetchAll();
        $location_cr_name = $location_cr_array[0][loc_name];

	$stmt = $dbh->prepare("INSERT INTO `cash_book`
		                          (`cash_book_group_id`,
					   `cash_book_source`,
		                           `location_id`,
		                           `personel_id`,
		                           `cash_report_only`,
		                           `type`,
		                           `date`,
		                           `desc`,
		                           `pay_cash`,
		                           `value`,
		                           `currency`)

		                   VALUES ('23',
					   '".$last_insert_id."',
		                           '14',
		                           '".$personel_id."',
		                           '1',
		                           '1',
		                           '".$date."',
		                           'from ".$location_cr_name."',
		                           '".$cash_cr_in."',
		                           '".$cash_cr_out."',
		                           '".$currency."')
		                   ");
	$stmt -> execute();
	echo '<p>Data add success</p>';
	$last_insert_id = $dbh->lastInsertId();
	} // DO USUNIECIA PO WYSTARTOWANIU cash movement

    if($cash_book_group_id==45 and $type==2) // DO USUNIECIA PO WYSTARTOWANIU cash movement
	{

	$cash_cr_in = $cash_out;

	$stmt_cr_loc = $dbh->prepare("SELECT location.name as loc_name,location.id_location
		                      FROM location
		                      WHERE location.id_location = ".$location_id."
		                      ORDER BY location.name");
	$stmt_cr_loc -> execute();
	$location_cr_array = $stmt_cr_loc->fetchAll();
        $location_cr_name = $location_cr_array[0][loc_name];

	$stmt = $dbh->prepare("INSERT INTO `cash_book`
		                          (`cash_book_group_id`,
					   `cash_book_source`,
		                           `location_id`,
		                           `personel_id`,
		                           `cash_report_only`,
		                           `type`,
		                           `date`,
		                           `desc`,
		                           `pay_cash`,
		                           `value`,
		                           `currency`)

		                   VALUES ('45',
					   '".$last_insert_id."',
		                           '15',
		                           '".$personel_id."',
		                           '1',
		                           '1',
		                           '".$date."',
		                           'from ".$location_cr_name."',
		                           '".$cash_cr_in."',
		                           '".$cash_cr_out."',
		                           '".$currency."')
		                   ");
	$stmt -> execute();
	echo '<p>Data add success</p>';
	$last_insert_id = $dbh->lastInsertId();
	} // DO USUNIECIA PO WYSTARTOWANIU cash movement

    if($cash_book_group_id==24 and $type==2)
	{

	$cash_cr_in = $cash_out;

	$stmt_cr_loc = $dbh->prepare("SELECT location.name as loc_name,location.id_location
		                      FROM location
		                      WHERE location.id_location = ".$location_id."
		                      ORDER BY location.name");
	$stmt_cr_loc -> execute();
	$location_cr_array = $stmt_cr_loc->fetchAll();
        $location_cr_name = $location_cr_array[0][loc_name];

	$stmt = $dbh->prepare("INSERT INTO `cash_book`
		                          (`cash_book_group_id`,
					   `cash_book_source`,
		                           `location_id`,
		                           `personel_id`,
		                           `cash_report_only`,
		                           `type`,
		                           `date`,
		                           `desc`,
		                           `pay_cash`,
		                           `value`,
		                           `currency`)

		                   VALUES ('24',
					   '".$last_insert_id."',
		                           '".$to_location_id."',
		                           '".$personel_id."',
		                           '1',
		                           '1',
		                           '".$date."',
		                           'from ".$location_cr_name."',
		                           '".$cash_cr_in."',
		                           '".$cash_cr_out."',
		                           '".$currency."')
		                   ");
	$stmt -> execute();
	echo '<p>Data add success</p>';
	$last_insert_id = $dbh->lastInsertId();
	}


    }
elseif($_POST['source_action']=='edit' and $_SESSION['permission']=='rw')
    {

    $stmt = $dbh->prepare("UPDATE `cash_book` SET

                           `desc` = '".$desc."',
                           `pay_cash` = '".$cash_in."',
                           `value` = '".$cash_out."'

                           WHERE `cash_book`.`id_cash_book` = '".$post_id."'
                           ");

    $stmt -> execute();


    $cash_cr_in = $cash_out;

    $stmt = $dbh->prepare("UPDATE `cash_book` SET

                           `pay_cash` = '".$cash_cr_in."',
                           `value` = '".$cash_cr_out."'

                           WHERE `cash_book`.`cash_book_source` = '".$post_id."'
                           ");

    $stmt -> execute();


    echo '<p>Data change success</p>';
    }
elseif($_GET['action']=='del' and $_SESSION['location_id_limit']==0 and $_SESSION['permission']=='rw')
    {
    $post_id = $_GET['id'];

    $stmt = $dbh->prepare("DELETE FROM `cash_book` WHERE `cash_book`.`id_cash_book` = '".$post_id."' ");

    $stmt -> execute();

    $stmt = $dbh->prepare("DELETE FROM `cash_book` WHERE `cash_book`.`cash_book_source` = '".$post_id."' ");

    $stmt -> execute();

    echo '<p>Data delete success</p>';



    }


$target_table = 'cash_book';
$txt = 'cash_report';


if($_POST['source_action']=='add')
    $target_id = $last_insert_id;
else
    $target_id = $post_id;

if($_GET['action']=='del')
    $target_action = $_GET['action'];
else
    $target_action = $_POST['source_action'];

$stmt = $dbh->prepare("INSERT INTO `logs` (`personel_id`,`ip`,`type`,`target_table`,`target_id`,`txt`)
             VALUES (
             '".$_SESSION['user_id']."',
             '".$_SERVER['REMOTE_ADDR']."',
             '".$target_action."',
             '".$target_table."',
             '".$target_id."',
             '".$txt."'
             )");

$stmt -> execute();


header('location: cash_report.php');
exit;

?>

