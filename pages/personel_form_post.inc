<?php

$post_id = $_POST[id];
$name = $_POST[name];
$email = $_POST[email];
$permission = $_POST[permission];
$timezone = $_POST[timezone];
$login = $_POST[login];
$password = hash('sha256', $_POST[password]);





if($_POST['source_action']=='add' and $_SESSION['permission']=='rw')
    {
    $stmt = $dbh->prepare("INSERT INTO `personel`
                                  (`name`,`email`,`login`,`password`,`permission`,`timezone`)
                           VALUES ('".$name."','".$email."','".$login."','".$password."','".$permission."','".$timezone."')
                           ");
    $stmt -> execute();

    echo '<p>Data add success</p>';

    }
elseif($_POST['source_action']=='edit' and $_SESSION['permission']=='rw')
    {

    if($_POST[password]<>'')
       $pass_upd = "`password` = '".$password."', ";

    $stmt = $dbh->prepare("UPDATE `personel` SET

                           `name` = '".$name."',
                           `login` = '".$login."',
                           `timezone` = '".$timezone."',
                           `permission` = '".$permission."',
                           ".$pass_upd."
                           `email` = '".$email."'

                           WHERE `personel`.`id_personel` = '".$post_id."'
                           ");

    $stmt -> execute();
    echo '<p>Data change success</p>';
    }
elseif($_GET['action']=='del' and $_SESSION['permission']=='rw')
    {
    $post_id = $_GET['id'];


    $stmt = $dbh->prepare("DELETE FROM `personel` WHERE `personel`.`id_personel` = '".$post_id."' ");

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


header('location: personel.php');
exit;

?>

