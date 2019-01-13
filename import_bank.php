<?php

$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');
include('inc/nav.inc');
?>



<form method="post" enctype="multipart/form-data">
Upload File: <input type="file" name="spreadsheet"/>
<input type="submit" name="submit" value="Submit" />
</form>
<!--<textarea rows="40" cols="200">-->
<table cellpadding=10 border=1>

<?php

//include('inc/db_config.inc');


$mysql_active = 1;


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
    $filename = strtoupper(pathinfo($_FILES['spreadsheet']['name'], PATHINFO_BASENAME));
    
//    if($extension == 'XLSX' || $extension == 'xlsx' || $extension == 'csv'){

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
        for ($row = 1; $row <= $highestRow; $row++){ 
                //  Read a row of data into an array
                $rowData = $sheet->rangeToArray('A' . $row . ':' . $highestColumn . $row, NULL, TRUE, FALSE);
                
                if($rowData[0][0]<>'')
                    {
                        //Post Date,Value Date,Branch,Journal No.,Description,Debit,Credit,


                        $account_no = str_replace('.CSV','',$filename);
                        $post_date = '20'.substr($rowData[0][0],6,2).'-'.substr($rowData[0][0],3,2).'-'.substr($rowData[0][0],0,2).'';
                        $value_date = '20'.substr($rowData[0][1],6,2).'-'.substr($rowData[0][1],3,2).'-'.substr($rowData[0][1],0,2).'';
                        $branch = $rowData[0][2];
                        $journal = $rowData[0][3];
                        $description_pre = explode('|',$rowData[0][4]);
                        $debit = str_replace(array(',','.00'),'',$rowData[0][5]);
                        $credit = str_replace(array(',','.00'),'',$rowData[0][6]);

                        $currency_code = '';
                        if($account_no=='380251201')
                            $currency_code = 'USD';
                        if($account_no=='359738937')
                            $currency_code = 'IDR';

                        $type = $description_pre[0];
                        $description = $description_pre[1].' '.$description_pre[2].' '.$description_pre[3].' '.$description_pre[4].' '.$description_pre[5];

                            $stmt = $dbh->prepare("INSERT INTO `bank_statement`
                                                            (`account_no`,
                                                            `post_date`,
                                                            `value_date`,
                                                            `branch`,
                                                            `journal_no`,
                                                            `type`,
                                                            `description`,
                                                            `debit`,
                                                            `credit`,
                                                            `currency_code`
                                                            )

                                                    VALUES ('".$account_no."',
                                                            '".$post_date."',
                                                            '".$value_date."',
                                                            '".$branch."',
                                                            '".$journal."',
                                                            '".$type."',
                                                            '".addslashes($description)."',
                                                            '".$debit."',
                                                            '".$credit."',
                                                            '".$currency_code."'
                                                            )
                                                    ");
                                
                                if($mysql_active==1) $stmt -> execute();
                                
                                $insert_id = $dbh->lastInsertId();                                
                            

                            echo '<tr>
                                        <td>'.$account_no.'</td>
                                        <td>'.$post_date.'</td>
                                        <td>'.$value_date.'</td>
                                        <td>'.$branch.'</td>
                                        <td>'.$journal.'</td>
                                        <td>'.$type.'</td>
                                        
                                        ';
                                 
                                            echo '<td>'.$description.'</td>';


                            echo '


                                        <td>'.$debit.'</td>
                                        <td>'.$credit.'</td>
                                </tr>'."&#13;&#10;";

            }
        }
    }
    else{
        echo "<font color=red>Please upload an XLSX or CSV file</font>";
    }
}
else{
    echo $_FILES['spreadsheet']['error'];
}
}
//}

?>
<!--</textarea>-->
</table>
</div>


<?php include('inc/footer.inc'); ?>