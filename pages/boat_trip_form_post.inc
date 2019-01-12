<?php

$post_id = $_POST[id];
$date_from = $_POST[date_from];
$date_to = $_POST[date_to];
$txt_route = $_POST[txt_route];
$price_cabin_a = str_replace(',','',$_POST[price_cabin_a]);
$txt_cabin_a = $_POST[txt_cabin_a];
$price_cabin_b = str_replace(',','',$_POST[price_cabin_b]);
$txt_cabin_b = $_POST[txt_cabin_b];
$price_cabin_c = str_replace(',','',$_POST[price_cabin_c]);
$txt_cabin_c = $_POST[txt_cabin_c];
$price_cabin_d = str_replace(',','',$_POST[price_cabin_d]);
$txt_cabin_d = $_POST[txt_cabin_d];
$price_cabin_e = str_replace(',','',$_POST[price_cabin_e]);
$txt_cabin_e = $_POST[txt_cabin_e];



if($_POST['source_action']=='add' and $_SESSION['permission']=='rw')
    {
    $stmt = $dbh->prepare("INSERT INTO `boat_trip`
                                  (
                                  `date_from`,
                                  `date_to`,
                                  `txt_route`,
                                  `price_cabin_a`,
                                  `txt_cabin_a`,
                                  `price_cabin_b`,
                                  `txt_cabin_b`,
                                  `price_cabin_c`,
                                  `txt_cabin_c`,
                                  `price_cabin_d`,
                                  `txt_cabin_d`,
                                  `price_cabin_e`,
                                  `txt_cabin_e`
                                  )
                           VALUES (
                                  '".$date_from."',
                                  '".$date_to."',
                                  '".$txt_route."',
                                  '".$price_cabin_a."',
                                  '".$txt_cabin_a."',
                                  '".$price_cabin_b."',
                                  '".$txt_cabin_b."',
                                  '".$price_cabin_c."',
                                  '".$txt_cabin_c."',
                                  '".$price_cabin_d."',
                                  '".$txt_cabin_d."',
                                  '".$price_cabin_e."',
                                  '".$txt_cabin_e."')
                           ");
    $stmt -> execute();
    echo '<p>Data add success</p>';

    }
elseif($_POST['source_action']=='edit' and $_SESSION['permission']=='rw')
    {


    $stmt = $dbh->prepare("UPDATE `boat_trip` SET

                           `date_from` = '".$date_from."',
                           `date_to` = '".$date_to."',
                           `txt_route` = '".$txt_route."',
                           `price_cabin_a` = '".$price_cabin_a."',
                           `txt_cabin_a` = '".$txt_cabin_a."',
                           `price_cabin_b` = '".$price_cabin_b."',
                           `txt_cabin_b` = '".$txt_cabin_b."',
                           `price_cabin_c` = '".$price_cabin_c."',
                           `txt_cabin_c` = '".$txt_cabin_c."',
                           `price_cabin_d` = '".$price_cabin_d."',
                           `txt_cabin_d` = '".$txt_cabin_d."',
                           `price_cabin_e` = '".$price_cabin_e."',
                           `txt_cabin_e` = '".$txt_cabin_e."'

                           WHERE `boat_trip`.`id_boat_trip` = '".$post_id."'
                           ");

    $stmt -> execute();
    echo '<p>Data change success</p>';
    }
elseif($_GET['action']=='del' and $_SESSION['permission']=='rw')
    {
    $post_id = $_GET['id'];


    $stmt = $dbh->prepare("DELETE FROM `boat_trip` WHERE `boat_trip`.`id_boat_trip` = '".$post_id."' ");

    $stmt -> execute();
    echo '<p>Data delete success</p>';
    }

$stmt = $dbh->prepare("INSERT INTO `logs` (`personel_id`,`ip`,`type`,`txt`)
             VALUES (
             '".$_SESSION['user_id']."',
             '".$_SERVER['REMOTE_ADDR']."',
             '".$_POST['source_action']."',
             'boat_trip id=".$post_id."'
             )");
$stmt -> execute();

header('location: boat_trip.php');
exit;

?>

