<?php

include('inc/calc_cur.inc');
$sys_currency = 'IDR';

$post_id = $_POST['id'];
$foreign_table = $_POST['foreign_table'];
$foreign_id = $_POST['foreign_id'];
$value = str_replace(',','.',$_POST['value']);
$bank_statement_id = $_POST['bank_statement_id'];

$stmt = $dbh->prepare("SELECT bank_statement.value_date, bank_statement.currency_code
FROM bank_statement
WHERE bank_statement.id_bank_statement = $bank_statement_id ");
$stmt -> execute();
$bs_data = $stmt->fetchAll();  

$bank_statement_date = $bs_data[0]['value_date'];
$bank_statement_currency = $bs_data[0]['currency_code'];

$value_foreign = conv_currency($value,'from',$bank_statement_currency,$bank_statement_date,$dbh);



if($_POST['source_action']=='add' and $_SESSION['permission']=='rw')
    {
    $stmt = $dbh->prepare("INSERT INTO `settlements`
                                  (`personel_id`,`foreign_table`,`foreign_id`,`value`,`value_foreign`,`bank_statement_id`)
                           VALUES ('".$_SESSION['user_id']."','".$foreign_table."','".$foreign_id."','".$value."','".$value_foreign."','".$bank_statement_id."')
                           ");
    $stmt -> execute();

    echo '<p>Data add success</p>';

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

