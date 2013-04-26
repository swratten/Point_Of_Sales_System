SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

CREATE SCHEMA IF NOT EXISTS `thebusiness` DEFAULT CHARACTER SET latin1 ;
USE `thebusiness` ;

-- -----------------------------------------------------
-- Table `thebusiness`.`categories`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `thebusiness`.`categories` (
  `ID` VARCHAR(45) NOT NULL ,
  `Name` VARCHAR(45) NOT NULL ,
  `AltCode` INT(11) NOT NULL ,
  PRIMARY KEY (`ID`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1;


-- -----------------------------------------------------
-- Table `thebusiness`.`products`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `thebusiness`.`products` (
  `ID` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT ,
  `Product_ID` VARCHAR(45) NOT NULL ,
  `Product_Name` VARCHAR(45) NOT NULL ,
  `Product_Description` VARCHAR(45) NULL DEFAULT NULL ,
  `Product_Category` VARCHAR(45) NOT NULL ,
  `Product_Price` DOUBLE NOT NULL ,
  `Product_Stock` INT(11) NOT NULL ,
  PRIMARY KEY (`ID`) ,
  INDEX `FK_Product_Price` (`Product_Price` ASC) ,
  INDEX `FK_Product_ID` (`Product_ID` ASC) ,
  INDEX `FK_Cat` (`Product_Category` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1;


-- -----------------------------------------------------
-- Table `thebusiness`.`orders`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `thebusiness`.`orders` (
  `ID` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT ,
  `Order_Date` VARCHAR(45) NOT NULL ,
  `Order_Price` DOUBLE NOT NULL ,
  `Prod_ID` INT(11) UNSIGNED NOT NULL ,
  `Order_Time` TIME NOT NULL ,
  `Order_ID` INT(11) NOT NULL ,
  `Qty` INT(11) NOT NULL ,
  PRIMARY KEY (`ID`) ,
  INDEX `FK_Order_ID` (`Order_ID` ASC) ,
  INDEX `FK_Prod_ID_idx` (`Prod_ID` ASC) ,
  INDEX `FK_Prod_ID_idx1` (`Prod_ID` ASC) ,
  CONSTRAINT `FK_Prod_ID`
    FOREIGN KEY (`Prod_ID` )
    REFERENCES `thebusiness`.`products` (`ID` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1;


-- -----------------------------------------------------
-- Table `thebusiness`.`payment`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `thebusiness`.`payment` (
  `ID` INT(11) NOT NULL AUTO_INCREMENT ,
  `Order_ID` INT(11) NOT NULL ,
  `Eftpos` DOUBLE NOT NULL ,
  `Cash` DOUBLE NOT NULL ,
  PRIMARY KEY (`ID`) ,
  INDEX `FK_Order_ID_P_idx` (`Order_ID` ASC) ,
  CONSTRAINT `FK_Order_ID_P`
    FOREIGN KEY (`Order_ID` )
    REFERENCES `thebusiness`.`orders` (`Order_ID` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1;

USE `thebusiness` ;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
