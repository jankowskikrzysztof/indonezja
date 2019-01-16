<?php

$post_id = $_POST['id'];
$foreign_table = $_POST['foreign_table'];
$foreign_id = $_POST['foreign_id'];
$value = str_replace(',','.',$_POST['value']);
$bank_statement_id = $_POST['bank_statement_id'];




if($_POST['source_action']=='add' and $_SESSION['permission']=='rw')
    {
    $stmt = $dbh->prepare("INSERT INTO `settlements`
                                  (`personel_id`,`foreign_table`,`foreign_id`,`value`,`bank_statement_id`)
                           VALUES ('".$_SESSION['user_id']."','".$foreign_table."','".$foreign_id."','".$value."','".$bank_statement_id."')
                           ");
    $stmt -> execute();

    echo '<p>Data add success</p>';

    }
elseif($_POST['source_action']=='edit' and $_SESSION['permission']=='rw')
    {

    if($_POST[password]<>'')
       $pass_upd = "`password` = '".$password."', ";

    $stmt = $dbh->prepare("UPDATE `settlements` SET

                           `personel_id` = '".$_SESSION['user_id']."',
                           `foreign_table` = '".$foreign_table."',
                           `foreign_id` = '".$foreign_id."',
                           `value` = '".$value."',
                           `bank_statement_id` = '".$bank_statement_id."'

                           WHERE `settlements`.`id_settlements` = '".$post_id."'
                           ");

    $stmt -> execute();
    echo '<p>Data change success</p>';
    }
elseif($_GET['action']=='del' and $_SESSION['permission']=='rw' and ($_SESSION['user_id'] == 6 or $_SESSION['user_id'] == 1))
    {
    $post_id = $_GET['id'];


    $stmt = $dbh->prepare("DELETE FROM `settlements` WHERE `settlements`.`id_settlements` = '".$post_id."' ");

    $stmt -> execute();
    echo '<p>Data delete success</p>';
    }

$stmt = $dbh->prepare("INSERT INTO `logs` (`personel_id`,`ip`,`type`,`txt`)
             VALUES (
             '".$_SESSION['user_id']."',
             '".$_SERVER['REMOTE_ADDR']."',
             '".$_POST['source_action']."',
             'personel id=".$post_id."'
             )");
$stmt -> execute();


header('location: index.php');
exit;

?>

