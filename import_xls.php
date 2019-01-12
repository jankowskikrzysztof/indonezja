<html>
<header>
 <title>Import Faktur Empress</title>
 <meta charset="utf-8">

<link rel="stylesheet" href="http://yui.yahooapis.com/pure/0.6.0/pure-min.css">
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.4.0/css/font-awesome.min.css">

<meta name="viewport" content="width=device-width, initial-scale=1">

</header>
<body><div>


<form method="post" enctype="multipart/form-data">
Upload File: <input type="file" name="spreadsheet"/>
<input type="submit" name="submit" value="Submit" />
</form>
<!--<textarea rows="40" cols="200">-->
<table cellpadding=10 border=1>

<?php

include('inc/db_config.inc');

include('inc/calc_cur.inc');
$sys_currency = 'IDR';


$mysql_active = 0;


ini_set('memory_limit', '-1');

define('EOL',(PHP_SAPI == 'cli') ? PHP_EOL : '<br />');
date_default_timezone_set('Europe/Warsaw');
require_once dirname(__FILE__) . '/inc/PHPExcel/IOFactory.php';

                //echo '<pre>';
                //var_dump($_FILES['spreadsheet']);
                //echo '</pre>';

 //Check valid spreadsheet has been uploaded
if(isset($_FILES['spreadsheet'])){
if($_FILES['spreadsheet']['tmp_name']){
if(!$_FILES['spreadsheet']['error'])
{

    $inputFile = $_FILES['spreadsheet']['tmp_name'];
    //$extension = strtoupper(pathinfo($inputFile, PATHINFO_EXTENSION));
    $extension = strtoupper(pathinfo($_FILES['spreadsheet']['name'], PATHINFO_EXTENSION));
    
    if($extension == 'XLSX' || $extension == 'xlsx' || $extension == 'ODS'){

        //Read spreadsheeet workbook
        try {
             $inputFileType = PHPExcel_IOFactory::identify($inputFile);
             $objReader = PHPExcel_IOFactory::createReader($inputFileType);
                 $objPHPExcel = $objReader->load($inputFile);
        } catch(Exception $e) {
                die($e->getMessage());
        }

        //Get worksheet dimensions
        $sheet = $objPHPExcel->getSheet(0); 
        $highestRow = $sheet->getHighestRow(); 
        $highestColumn = $sheet->getHighestColumn();

        //Loop through each row of the worksheet in turn
        for ($row = 2; $row <= $highestRow; $row++){ 
                //  Read a row of data into an array
                $rowData = $sheet->rangeToArray('A' . $row . ':' . $highestColumn . $row, NULL, TRUE, FALSE);
                
                if($rowData[0][0]<>'')
                    {
                            //usuwam nawiasy
                            $data_faktury = ''; 
                            $data_faktury = date('Y-m-d', PHPExcel_Shared_Date::ExcelToPHP($rowData[0][0]));
                            $numer_faktury = ''; 
                            $numer_faktury = $rowData[0][1];
                            $klient = ''; 
                            $klient = $rowData[0][2];
                            $bookeria = ''; 
                            $bookeria = $rowData[0][3];
                            $nr_rezerwacji = ''; 
                            $nr_rezerwacji = $rowData[0][4];
                            $data_rozp_tripu = ''; 
                            $data_rozp_tripu = date('Y-m-d', PHPExcel_Shared_Date::ExcelToPHP($rowData[0][5]));
                            $waluta = ''; 
                            $waluta = $rowData[0][6];
                            $kabina = ''; 
                            $kabina = $rowData[0][7];
                            if(!$kabina) $kabina = 'A';
                            $kwota = ''; 
                            $kwota = $rowData[0][8];


                            $ilosc_na_cab_trip[$data_rozp_tripu][$kabina]++;
                            $blad_kabiny = '';
                            if($ilosc_na_cab_trip[$data_rozp_tripu][$kabina]>3)
                                $blad_kabiny = '<font color=red><b>';

                            //mapping

                            if($bookeria == 'Liveaboard.com')
                                $bookeria = 1;
                            elseif($bookeria == 'Diviac travel')
                                $bookeria = 2;
                            elseif($bookeria == 'Padi Travel')
                                $bookeria = 2;
                            elseif($bookeria == 'PADI T')
                                $bookeria = 2;
                            elseif($bookeria == 'Allwaysdive')
                                $bookeria = 5;
                            elseif($bookeria == 'Dive The World')
                                $bookeria = 7;
                            elseif($bookeria == 'Divebooker.com' or $bookeria == 'Divebooker')
                                $bookeria = 8;
                            elseif($bookeria == 'Flores Diving Center')
                                $bookeria = 9;
                            elseif($bookeria == 'Long Mu Tours')
                                $bookeria = 10;
                            elseif($bookeria == 'No Troubles - Just Bubbles')
                                $bookeria = 11;
                            elseif($bookeria == 'No Troubles, Just Bubbles')
                                $bookeria = 11;
                            elseif($bookeria == 'Scubaliveaboards.com')
                                $bookeria = 12;
                            elseif($bookeria == 'ScubaTravel Scandin')
                                $bookeria = 13;
                            elseif($bookeria == 'Alpha Dive')
                                $bookeria = 14;
                            elseif(in_array($bookeria, array('7 pax',
                                                '',
                                                'Aaron Hand',
                                                'charter ', 
                                                'charter group',
                                                'charter group 10pax',
                                                'Maria Mulder',
                                                'Mrcel Wals',
                                                'Paweł Smolka',
                                                'priv/charter',
                                                'priv')))
                                $bookeria = 0;


                            //szukam bookerii

                            $book_search = 'SELECT * FROM booker WHERE id_booker=\''.$bookeria.'\'';
                            $stmt = $dbh->prepare($book_search);
                            $stmt -> execute();
                            $book_array = $stmt->fetchAll();

                            $booker_id = '';
                            $commission = '';

                            foreach($book_array as $row_sql)
                                {
                                    $booker_id = $row_sql['id_booker'];
                                    $bookeria_commission = $row_sql['commission'];
                                }
                            

                                
                            //identyfikacja tripu

                            $trip_search = 'SELECT * FROM boat_trip WHERE date_from=\''.$data_rozp_tripu.'\'';
                            $stmt = $dbh->prepare($trip_search);
                            $stmt -> execute();
                            $trip_array = $stmt->fetchAll();
                            foreach($trip_array as $row_sql)
                                {
                                    $trip_id = $row_sql['id_boat_trip'];
                                    $txt_route = $row_sql['txt_route'];
                                }

                            // klient tripu


                            $stmt = $dbh->prepare("INSERT INTO `client`
                                                            (`name`,
                                                            `buyer`
                                                            )

                                                    VALUES ('".$klient."',
                                                            'import'
                                                            )
                                                    ");
                                
                                if($mysql_active==1) $stmt -> execute();
                                
                                $insert_id = $dbh->lastInsertId();                                
                            
                            $klient_id = $insert_id;


                            //identyfikacja aktywnosci

                            $act_search = 'SELECT * FROM activity WHERE shortcut=\''.$kabina.''.$ilosc_na_cab_trip[$data_rozp_tripu][$kabina].'\'';
                            $stmt = $dbh->prepare($act_search);
                            $stmt -> execute();
                            $act_array = $stmt->fetchAll();

                            $act_name = '';
                            $activity_id = '';

                            foreach($act_array as $row_sql)
                                {
                                    $cabin_id = $row_sql['id_activity'];

                                    //echo $cabin_id;

                                    $act_name = $row_sql['name'];
                                }

                            if($last_numer_faktury==$numer_faktury)
                                $multi_cabin = 1;
                            else
                                $multi_cabin = 0;
                                
                            $sql_cb = '';
                            $sql_cb_item = '';


                            if($multi_cabin==1)
                                $font_faktura = '<font color=red><b>';
                            else
                                $font_faktura = '';




                                $location_id = 21;
                                $cash_book_group_id = 14;
                                $personel_id = 1;
                                $client_id = $klient_id;
                                $bill_no = $numer_faktury;
                                $boat_trip_id = $trip_id;
                                $date = $data_faktury;
                                //$activity_count = 1;
                                $activity_id[0] = $cabin_id;
                                $activity_count = count($activity_id);
                                $price[0] = $kwota;
                                $currency = $waluta;
                                $type = 1;
                                $value_before_discount = $kwota;
                                $booker_id = $bookeria;
                                $booker_commission = $kwota*$bookeria_commission/100;
                                $booker_perc = $bookeria_commission;
                                $value = $value_before_discount - $booker_commission; //total_act
                                //$pay_bank_part = $value;
                                //$total_payment = $value;
                                $still_to_pay = $value;
                                $booking_no = $nr_rezerwacji;








                            for($i=0;$i<$activity_count;$i++)
                                {
                                if($currency <> $sys_currency)
                                {
                                $price_currency[$i] = str_replace(',','',$price[$i]);
                                $price[$i] = conv_currency(str_replace(',','',$price[$i]),'from',$currency,$date,$dbh);
                                $item_currency_second[$i] = $currency;
                                $item_currency[$i] = $sys_currency;
                                }
                                }
                        
                            if($currency <> $sys_currency)
                                {
                                $booker_commission_currency = $booker_commission;
                                $booker_commission = conv_currency($booker_commission,'from',$currency,$date,$dbh);
                        
                                $instructor_commission_currency = $instructor_commission;
                                $instructor_commission = conv_currency($instructor_commission,'from',$currency,$date,$dbh);
                        
                        
                                $value_currency = $value;
                                $value = conv_currency($value,'from',$currency,$date,$dbh);
                        
                                $value_before_discount_currency = $value_before_discount;
                                $value_before_discount = conv_currency($value_before_discount,'from',$currency,$date,$dbh);
                        
                                $currency_second = $currency;
                                $currency = $sys_currency;
                                }
                        
                        /////////////////////////////////// ZAMIANA WALUT
                        
                        
                        
                            $insert = "INSERT INTO `cash_book`
                                                        (`cash_book_group_id`,
                                                            `location_id`,
                                                            `personel_id`,
                                                            `client_id`,
                                                            `boat_trip_id`,
                                                            `type`,
                                                            `type_cost`,
                                                            `date`,
                                                            `bill_no`,
                                                            `desc`,
                                                            `value`,
                                                            `value_currency`,
                                                            `currency`,
                                                            `currency_second`,
                                                            `discount`,
                                                            `discount_currency`,
                                                            `value_before_discount`,
                                                            `value_before_discount_currency`,
                                                            `instructor_id`,
                                                            `instructor_perc`,
                                                            `instructor_commission`,
                                                            `instructor_commission_currency`,
                                                            `booker_id`,
                                                            `booker_perc`,
                                                            `booker_commission`,
                                                            `booker_commission_currency`,
                                                            `booking_no`,
                                                            `cash_report_only`,
                                                            `pay_cash`,
                                                            `pay_cash_currency`,
                                                            `pay_cash_part`,
                                                            `pay_cash_part_currency`,
                                                            `pay_cash_deposit`,
                                                            `pay_cash_deposit_currency`,
                                                            `date_pay_deposit`,
                                                            `pay_cash_deposit2`,
                                                            `pay_cash_deposit2_currency`,
                                                            `date_pay_deposit2`,
                                                            `pay_cash_deposit3`,
                                                            `pay_cash_deposit3_currency`,
                                                            `date_pay_deposit3`,
                                                            `pay_cash_deposit4`,
                                                            `pay_cash_deposit4_currency`,
                                                            `date_pay_deposit4`,
                                                            `pay_bank_part`,
                                                            `pay_bank_part_currency`,
                                                            `pay_bank_deposit`,
                                                            `pay_bank_deposit_currency`,
                                                            `pay_bank_deposit2`,
                                                            `pay_bank_deposit2_currency`,
                                                            `pay_bank_deposit3`,
                                                            `pay_bank_deposit3_currency`,
                                                            `pay_bank_deposit4`,
                                                            `pay_bank_deposit4_currency`,
                                                            `pay_roomcharge`,
                                                            `pay_roomcharge_currency`,
                                                            `pay_creditcard`,
                                                            `pay_creditcard_currency`,
                                                            `pay_paypal`,
                                                            `pay_paypal_currency`,
                                                            `pay_bank`,
                                                            `pay_bank_currency`
                                                            )
                        
                                                    VALUES ('".$cash_book_group_id."',
                                                            '".$location_id."',
                                                            '".$personel_id."',
                                                            '".$client_id."',
                                                            '".$boat_trip_id."',
                                                            '".$type."',
                                                            '".$type_cost."',
                                                            '".$date."',
                                                            '".$bill_no."',
                                                            '".$desc."',
                                                            '".$value."',
                                                            '".$value_currency."',
                                                            '".$currency."',
                                                            '".$currency_second."',
                                                            '".$discount."',
                                                            '".$discount_currency."',
                                                            '".$value_before_discount."',
                                                            '".$value_before_discount_currency."',
                                                            '".$instructor_id[0]."',
                                                            '".$instructor_perc."',
                                                            '".$instructor_commission."',
                                                            '".$instructor_commission_currency."',
                                                            '".$booker_id."',
                                                            '".$booker_perc."',
                                                            '".$booker_commission."',
                                                            '".$booker_commission_currency."',
                                                            '".$booking_no."',
                                                            '0',
                                                            '".$pay_cash."',
                                                            '".$pay_cash_currency."',
                                                            '".$pay_cash_part."',
                                                            '".$pay_cash_part_currency."',
                                                            '".$pay_cash_deposit."',
                                                            '".$pay_cash_deposit_currency."',
                                                            '".$date_pay_deposit."',
                                                            '".$pay_cash_deposit2."',
                                                            '".$pay_cash_deposit2_currency."',
                                                            '".$date_pay_deposit2."',
                                                            '".$pay_cash_deposit3."',
                                                            '".$pay_cash_deposit3_currency."',
                                                            '".$date_pay_deposit3."',
                                                            '".$pay_cash_deposit4."',
                                                            '".$pay_cash_deposit4_currency."',
                                                            '".$date_pay_deposit4."',
                                                            '".$pay_bank_part."',
                                                            '".$pay_bank_part_currency."',
                                                            '".$pay_bank_deposit."',
                                                            '".$pay_bank_deposit_currency."',
                                                            '".$pay_bank_deposit2."',
                                                            '".$pay_bank_deposit2_currency."',
                                                            '".$pay_bank_deposit3."',
                                                            '".$pay_bank_deposit3_currency."',
                                                            '".$pay_bank_deposit4."',
                                                            '".$pay_bank_deposit4_currency."',
                                                            '".$pay_roomcharge."',
                                                            '".$pay_roomcharge_currency."',
                                                            '".$pay_creditcard."',
                                                            '".$pay_creditcard_currency."',
                                                            '".$pay_paypal."',
                                                            '".$pay_paypal_currency."',
                                                            '".$pay_bank."',
                                                            '".$pay_bank_currency."'
                                                            )";
                        
                            $sql_cb = $insert;
                            $stmt = $dbh->prepare($insert);
                            
                            
                            if($mysql_active==1) $stmt -> execute();
                        
                            //echo '<br><br><pre>'.$insert.'<br><br>';
                        
                            $id = $dbh->lastInsertId();
                            $last_insert_id = $dbh->lastInsertId();
                        
                            for($i=0;$i<$activity_count;$i++)
                                {
                                //echo '<br>'.$i.'<br>';
                                $insert = "INSERT INTO `cash_book_item`
                                                        (`cash_book_id`,
                                                            `activity_id`,
                                                            `amount`,
                                                            `value`,
                                                            `value_currency`,
                                                            `item_currency`,
                                                            `item_currency_second`
                                                            )
                        
                                                    VALUES ('".$id."',
                                                            '".$activity_id[$i]."',
                                                            '".$pcs_no[$i]."',
                                                            '".str_replace(',','',$price[$i])."',
                                                            '".str_replace(',','',$price_currency[$i])."',
                                                            '".$item_currency[$i]."',
                                                            '".$item_currency_second[$i]."'
                                                            )";
                                $sql_cb_item = $insert;
                                //echo $insert.'<br>';
                                $stmt = $dbh->prepare($insert);
                                if($mysql_active==1) $stmt -> execute();

                                }
                        

                        





                            if($value==0)
                                $value_err = ' bgcolor=red';
                            else
                                $value_err = '';

                            if($kwota==0)
                                $kwota_err = ' bgcolor=red';
                            else
                                $kwota_err = '';

                            if($trip_id==0)
                                $trip_id_err = ' bgcolor=red';
                            else
                                $trip_id_err = '';

                            if($cabin_id==0)
                                $cabin_id_err = ' bgcolor=red';
                            else
                                $cabin_id_err = '';

                            echo '<tr>
                                        <td>'.$font_faktura.$data_faktury.'</b></font></td>
                                        <td>'.$font_faktura.$numer_faktury.'</b></font></td>
                                        <td>'.$klient.'</td>
                                        <td>'.$bookeria.'</td>
                                        <td>'.$bookeria_commission.'</td>
                                        <td>'.$nr_rezerwacji.'</td>
                                        <td>'.$data_rozp_tripu.'</td>
                                        <td '.$trip_id_err.'>'.$trip_id.': '.$txt_route.'</td>
                                        <td>'.$waluta.'</td>
                                        <td>'.$blad_kabiny .$kabina.' '.$ilosc_na_cab_trip[$data_rozp_tripu][$kabina].'</b></font></td>
                                        <td '.$cabin_id_err.'>'.$act_name.' [ID: '.$cabin_id.']</td>
                                        <td '.$kwota_err.'>'.$kwota.'</td>  
                                        <td>'.$booker_commission.'</td>
                                        <td '.$value_err.'>'.$value.'</td>
                                        <td>'.$sql_cba.'</td>
                                        <td>'.$sql_cb_itema.'</td>                          
                                </tr>'."&#13;&#10;";

                    $last_numer_faktury = $numer_faktury;
            }
        }
    }
    else{
        echo "<font color=red>Please upload an XLSX or ODS file</font>";
    }
}
else{
    echo $_FILES['spreadsheet']['error'];
}
}
}

?>
<!--</textarea>-->
</table>
</div>
</body>
</html>