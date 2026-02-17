#!/usr/bin/perl
#
#  Copyright (c) 2011-2024 FastMail Pty Ltd. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in
#     the documentation and/or other materials provided with the
#     distribution.
#
#  3. The name "Fastmail Pty Ltd" must not be used to
#     endorse or promote products derived from this software without
#     prior written permission. For permission or any legal
#     details, please contact
#      FastMail Pty Ltd
#      PO Box 234
#      Collins St West 8007
#      Victoria
#      Australia
#
#  4. Redistributions of any form whatsoever must retain the following
#     acknowledgment:
#     "This product includes software developed by Fastmail Pty. Ltd."
#
#  FASTMAIL PTY LTD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
#  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY  AND FITNESS, IN NO
#  EVENT SHALL OPERA SOFTWARE AUSTRALIA BE LIABLE FOR ANY SPECIAL, INDIRECT
#  OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
#  USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
#  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
#  OF THIS SOFTWARE.
#

package Cassandane::Cyrus::JMAPTasks;
use strict;
use warnings;
use DateTime;
use JSON::XS;
use Net::CalDAVTalk 0.09;
use Data::Dumper;
use Data::GUID qw(guid_string);

use base qw(Cassandane::Cyrus::TestCase);
use Cassandane::Util::Log;

sub new
{
    my ($class, @args) = @_;
    my $config = Cassandane::Config->default()->clone();

    $config->set(caldav_realm => 'Cassandane',
                 caldav_historical_age => -1,
                 conversations => 'yes',
                 httpmodules => 'carddav caldav jmap',
                 httpallowcompress => 'no',
                 jmap_nonstandard_extensions => 'yes',
                 defaultdomain => 'example.com',
                 virtdomains => 'no');

    my $self = $class->SUPER::new({
        config => $config,
        jmap => 1,
        adminstore => 1,
        services => [ 'imap', 'http' ],
    }, @args);

    $self->needs('component', 'jmap');
    return $self;
}

sub set_up
{
    my ($self) = @_;
    $self->SUPER::set_up();
    if ($self->{_want}->{start_instances}) {
        $self->{jmap}->DefaultUsing([
            'urn:ietf:params:jmap:core',
            'urn:ietf:params:jmap:calendars',
            'urn:ietf:params:jmap:tasks',
            'https://cyrusimap.org/ns/jmap/calendars',
            'https://cyrusimap.org/ns/jmap/debug',
        ]);
    }
}

use Cassandane::Tiny::Loader;

1;
