<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE xsl:stylesheet [<!ENTITY nl "&#xa;">]><!--new line-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="text" encoding="utf-8"/>
<xsl:key name="mediatype" match="Page" use="@form" />

<xsl:template match="/">
    <xsl:text>Pocet obalok: </xsl:text>
    <xsl:value-of select="count(//Envelope)"/>

    <xsl:text>&nl;Pocet stran: </xsl:text>
    <xsl:value-of select="count(//Page)"/>

    <xsl:text>&nl;&nl;Pocty pouzitych typov papiera</xsl:text>
    <xsl:text>&nl;-----------------------------&nl;</xsl:text>

    <!-- Muenchian Method -->
    <xsl:for-each select="//Page[count(. | key('mediatype', @form)[1]) = 1]">
        <xsl:sort select="@tray" />
        <xsl:value-of select="@form" />
        <xsl:text> (tray </xsl:text>
        <xsl:value-of select="@tray" />
        <xsl:text>, seq </xsl:text>
        <xsl:value-of select="@seq" />
        <xsl:text>): </xsl:text>
        <xsl:value-of select="count(key('mediatype', @form))" />
        <xsl:text>&nl;</xsl:text>
    </xsl:for-each>

    <xsl:text>&nl;Triedenie HSS</xsl:text>
    <xsl:text>&nl;-------------</xsl:text>
    <xsl:text>&nl;nesprávne PSČ           : 0</xsl:text>
    <xsl:text>&nl;HSS Bratislava 022 m. p.: </xsl:text><xsl:value-of select="count(//Envelope[@hss='1'])"/>
    <xsl:text>&nl;HSS Bratislava 022      : </xsl:text><xsl:value-of select="count(//Envelope[@hss='2'])"/>
    <xsl:text>&nl;HSS Zvolen 022          : </xsl:text><xsl:value-of select="count(//Envelope[@hss='3'])"/>
    <xsl:text>&nl;HSS Žilina 022          : </xsl:text><xsl:value-of select="count(//Envelope[@hss='4'])"/>
    <xsl:text>&nl;HSS Košice 022          : </xsl:text><xsl:value-of select="count(//Envelope[@hss='5'])"/>
    <xsl:text>&nl;HSS 6 (nepouziva sa)    : </xsl:text><xsl:value-of select="count(//Envelope[@hss='6'])"/>
    <xsl:text>&nl;HSS 7 (nepouziva sa)    : </xsl:text><xsl:value-of select="count(//Envelope[@hss='7'])"/>
    <xsl:text>&nl;Doporučené              : </xsl:text><xsl:value-of select="count(//Envelope[@hss='8'])"/>
    <xsl:text>&nl;Zahraničie              : </xsl:text><xsl:value-of select="count(//Envelope[@hss='9'])"/>
    <xsl:text>&nl;</xsl:text>
</xsl:template>
</xsl:stylesheet>
