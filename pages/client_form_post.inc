<?php
ob_start();
error_reporting(E_ALL & ~E_NOTICE);

$post_id = $_POST[id];
$name = $_POST[name];
$date_of_born = $_POST[date_of_born];
$gender = $_POST[gender];
$nationality = $_POST[nationality];
$no_of_dives = $_POST[no_of_dives];
$passport = $_POST[passport];
$buyer = $_POST[buyer];
$nitrox = $_POST[nitrox];
$gear_to_rent = $_POST[gear_to_rent];
$remarks = $_POST[remarks];
$email = $_POST[email];
$park_fee = $_POST[park_fee];
$eqip_rental = $_POST[eqip_rental];

$back = $_POST['back'];
$sel_trip = $_POST['sel_trip'];




if($_POST['source_action']=='add' and $_SESSION['permission']=='rw')
    {
    $stmt = $dbh->prepare("INSERT INTO `client`
                                  (`name`,
                                   `date_of_born`,
                                   `gender`,
                                   `nationality`,
                                   `no_of_dives`,
                                   `passport`,
                                   `buyer`,
                                   `nitrox`,
                                   `gear_to_rent`,
                                   `remarks`,
                                   `email`,
                                   `park_fee`,
                                   `eqip_rental`)

                           VALUES ('".$name."',
                                   '".$date_of_born."',
                                   '".$gender."',
                                   '".$nationality."',
                                   '".$no_of_dives."',
                                   '".$passport."',
                                   '".$buyer."',
                                   '".$nitrox."',
                                   '".$gear_to_rent."',
                                   '".$remarks."',
                                   '".$email."',
                                   '".$park_fee."',
                                   '".$eqip_rental."'
                                   )
                           ");
    $stmt -> execute();
    $insert_id = $dbh->lastInsertId();
    echo '<p>Data ID '.$insert_id.' add success</p>';

    }
elseif($_POST['source_action']=='edit' and $_SESSION['permission']=='rw')
    {


    $stmt = $dbh->prepare("UPDATE `client` SET

                           `name` = '".$name."',
                           `date_of_born` = '".$date_of_born."',
                           `gender` = '".$gender."',
                           `nationality` = '".$nationality."',
                           `no_of_dives` = '".$no_of_dives."',
                           `passport` = '".$passport."',
                           `buyer` = '".$buyer."',
                           `nitrox` = '".$nitrox."',
                           `gear_to_rent` = '".$gear_to_rent."',
                           `remarks` = '".$remarks."',
                           `email` = '".$email."',
                           `park_fee` = '".$park_fee."',
                           `eqip_rental` = '".$eqip_rental."'

                           WHERE `client`.`id_client` = '".$post_id."'
                           ");

    $stmt -> execute();
    echo '<p>Data change success</p>';
    }
elseif($_GET['action']=='del' and $_SESSION['permission']=='rw')
    {
    $post_id = $_GET['id'];


    $stmt = $dbh->prepare("DELETE FROM `client` WHERE `client`.`id_client` = '".$post_id."' ");

    $stmt -> execute();
    echo '<p>Data delete success</p>';
    }

$stmt = $dbh->prepare("INSERT INTO `logs` (`personel_id`,`ip`,`type`,`txt`)
             VALUES (
             '".$_SESSION['user_id']."',
             '".$_SERVER['REMOTE_ADDR']."',
             '".$_POST['source_action']."',
             'client id=".$post_id."'
             )");
$stmt -> execute();

if($back=='emp_add')
    {
    //echo 'aaa';
    header("location:empress_income.php?action=add&sel_trip=".$sel_trip."&sel_client=".$insert_id);
    }
else
    header("location: client.php");

exit;

?>